#pragma once

#include "entity.hpp"

#include "../containers/allocator_concepts.hpp"
#include "../containers/fixed_array.hpp"

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace Engine::ECS {

// Stores one entity type in a dense, fixed-capacity array. Handles refer to
// stable slots, so swap-removing an entity never invalidates other handles.
template<typename EntityType, Containers::Allocator A>
class EntityManager {
public:
  using Handle = EntityHandle<EntityType>;

  explicit EntityManager(
    A& allocator,
    const std::size_t capacity = MAX_ENTITIES
  ) : entities_(allocator, checked_capacity(capacity)),
      dense_to_slot_(allocator, checked_capacity(capacity)),
      slots_(allocator, checked_capacity(capacity)),
      free_slot_head_(capacity == 0 ? INVALID_ENTITY_INDEX : 0)
  {
    for (std::uint32_t index = 0; index < capacity; ++index)
    {
      const std::uint32_t next = index + 1 < capacity
        ? index + 1
        : INVALID_ENTITY_INDEX;
      slots_.push_back(Slot{0, next});
    }
  }

  ~EntityManager() = default;

  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;
  EntityManager(EntityManager&&) = delete;
  EntityManager& operator=(EntityManager&&) = delete;

  template<typename... Args>
    requires std::constructible_from<EntityType, Args...>
  [[nodiscard]] Handle create(Args&&... args)
  {
    if (free_slot_head_ == INVALID_ENTITY_INDEX) [[unlikely]]
    {
      return INVALID_ENTITY<EntityType>;
    }

    const std::uint32_t slot_index = free_slot_head_;
    Slot& slot = slots_[slot_index];

    // Construct first: if EntityType's constructor throws, the free-list and
    // handle state are unchanged.
    entities_.emplace_back(std::forward<Args>(args)...);
    dense_to_slot_.push_back(slot_index);

    free_slot_head_ = slot.index;
    slot.index = static_cast<std::uint32_t>(entities_.size() - 1);
    return Handle{slot_index, slot.generation};
  }

  void destroy(const Handle entity)
    noexcept(std::is_nothrow_move_constructible_v<EntityType>)
  {
    if (!is_alive(entity)) [[unlikely]] return;

    Slot& removed_slot = slots_[entity.index];
    const std::uint32_t dense_index = removed_slot.index;
    const std::uint32_t last_dense_index =
      static_cast<std::uint32_t>(entities_.size() - 1);

    if (dense_index != last_dense_index)
    {
      const std::uint32_t moved_slot = dense_to_slot_[last_dense_index];
      slots_[moved_slot].index = dense_index;
    }

    entities_.swap_remove(dense_index);
    dense_to_slot_.swap_remove(dense_index);

    ++removed_slot.generation;
    removed_slot.index = free_slot_head_;
    free_slot_head_ = entity.index;
  }

  [[nodiscard]] bool is_alive(const Handle entity) const noexcept
  {
    if (entity.index >= slots_.size()) return false;

    const Slot& slot = slots_[entity.index];
    return slot.generation == entity.generation &&
           slot.index < entities_.size() &&
           dense_to_slot_[slot.index] == entity.index;
  }

  [[nodiscard]] EntityType* get(const Handle entity) noexcept
  {
    return is_alive(entity) ? &entities_[slots_[entity.index].index] : nullptr;
  }

  [[nodiscard]] const EntityType* get(const Handle entity) const noexcept
  {
    return is_alive(entity) ? &entities_[slots_[entity.index].index] : nullptr;
  }

  [[nodiscard]] std::size_t size() const noexcept { return entities_.size(); }
  [[nodiscard]] std::size_t capacity() const noexcept { return entities_.capacity(); }
  [[nodiscard]] bool empty() const noexcept { return entities_.size() == 0; }

  EntityType* begin() noexcept { return entities_.begin(); }
  const EntityType* begin() const noexcept { return entities_.begin(); }
  EntityType* end() noexcept { return entities_.end(); }
  const EntityType* end() const noexcept { return entities_.end(); }

private:
  struct Slot
  {
    std::uint32_t generation;
    // Dense-array index while alive; next free slot while dead.
    std::uint32_t index;
  };

  static std::size_t checked_capacity(const std::size_t capacity)
  {
    if (capacity > MAX_ENTITIES)
    {
      throw std::length_error("EntityManager capacity exceeds MAX_ENTITIES.");
    }
    return capacity;
  }

  Containers::FixedArray<EntityType, A> entities_;
  Containers::FixedArray<std::uint32_t, A> dense_to_slot_;
  Containers::FixedArray<Slot, A> slots_;
  std::uint32_t free_slot_head_;
};

} // namespace Engine::ECS

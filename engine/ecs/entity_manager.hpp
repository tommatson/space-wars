#pragma once

#include "entity.hpp"

#include "../containers/fixed_array.hpp"
#include "../memory/free_list_allocator.hpp"
#include "../containers/allocator_concepts.hpp"

namespace Engine::ECS {
template<Containers::Allocator A>
class EntityManager{
public:
  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  EntityManager(A allocator ) noexcept : allocator_{allocator} 
  {
  }
  ~EntityManager() = default;

  // ---------------------------------------------------------------------------
  // Delete Semantics
  // ---------------------------------------------------------------------------
  EntityManager(const EntityManager&) = delete;
  EntityManager& operator=(const EntityManager&) = delete;
  EntityManager(EntityManager&&) = delete;
  EntityManager& operator=(EntityManager&&) = delete;

  // ---------------------------------------------------------------------------
  // Core API
  // ---------------------------------------------------------------------------
  [[nodiscard]] Entity create() noexcept;
  void destroy(Entity entity) noexcept;
  [[nodiscard]] bool is_alive(Entity entity) const noexcept;

private:
  A allocator_;
  const std::size_t MAX_ENTITIES = 100;


  Containers::FixedArray<Entity, Memory::FreeListAllocator> entity_array_();


};

} // namespace Engine::ECS

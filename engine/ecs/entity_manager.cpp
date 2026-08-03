#include "entity_manager.hpp"

#include <cstdint>

namespace Engine::ECS {

EntityManager::EntityManager() noexcept
{
  for (std::uint32_t index = 0; index < MAX_ENTITIES; ++index)
  {
    free_indices_[MAX_ENTITIES - index - 1] = index;
  }
}

[[nodiscard]] Entity EntityManager::create() noexcept
{
  if (free_count_ == 0) [[unlikely]] return INVALID_ENTITY;

  const std::uint32_t index = free_indices_[--free_count_];
  alive_[index] = true;

  return Entity{index, generations_[index]};
}

void EntityManager::destroy(const Entity entity) noexcept
{
  if (!is_alive(entity)) [[unlikely]] return;

  alive_[entity.index] = false;
  ++generations_[entity.index];
  free_indices_[free_count_++] = entity.index;
}

[[nodiscard]] bool EntityManager::is_alive(const Entity entity) const noexcept
{
  return entity.index < MAX_ENTITIES &&
         alive_[entity.index] &&
         generations_[entity.index] == entity.generation;
}

} // namespace Engine::ECS

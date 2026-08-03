#pragma once

#include "entity.hpp"

#include <array>
#include <cstdint>

namespace Engine::ECS {

class EntityManager{
public:
  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  EntityManager() noexcept;
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
  std::array<std::uint32_t, MAX_ENTITIES> generations_{};
  std::array<bool, MAX_ENTITIES> alive_{};
  std::array<std::uint32_t, MAX_ENTITIES> free_indices_{};
  std::uint32_t free_count_ = MAX_ENTITIES;
};

} // namespace Engine::ECS

#pragma once

#include <cstdint>

namespace Engine::ECS {

constexpr std::uint32_t MAX_ENTITIES = 100'000;
constexpr std::uint32_t INVALID_ENTITY_INDEX = MAX_ENTITIES;

struct Entity
{
  std::uint32_t index;
  std::uint32_t generation;

  [[nodiscard]] constexpr bool operator==(const Entity&) const noexcept = default;
};

constexpr Entity INVALID_ENTITY{INVALID_ENTITY_INDEX, 0};

} // namespace Engine::ECS

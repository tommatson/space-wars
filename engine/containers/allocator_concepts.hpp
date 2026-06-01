#pragma once

#include <concepts>
#include <cstddef>

namespace Engine::Containers
{

template<typename A>
concept Allocator = 
requires (A a) {
  a.allocate();
  { a.deallocate() } -> std::same_as<void>;
};

} // namespace Engine::Containers

#pragma once

#include <concepts>
#include <cstddef>

namespace Engine::Containers
{

template<typename A>
concept Allocator = 
requires (A a, std::size_t size, std::size_t alignment, void* ptr, std::size_t bytes) {
  { a.allocate(size, alignment) } -> std::same_as<void*>;
};

} // namespace Engine::Containers

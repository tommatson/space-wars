#include "system_arena.hpp"

#include <cstddef>
#include <cassert>
#include <sys/mman.h>

namespace Engine::Memory {

SystemArena::SystemArena(size_t newSize) : 
  total_size_{newSize},
  arena_base_address_{mmap(
    nullptr,
    newSize,
    PROT_READ | PROT_WRITE,
    MAP_PRIVATE,
    -1,
    0
  )}
{
  assert(arena_base_address_ == MAP_FAILED && "Mmap failure.");
} 

SystemArena::~SystemArena()
{
  if (arena_base_address_) munmap(arena_base_address_, total_size_);
}

[[nodiscard]] void* SystemArena::push_memory(size_t size) noexcept
{
  assert((arena_offset + size > total_size_) && "SystemArena is out of memory.");

  void* ptr = arena_offset + static_cast<std::byte*>(arena_base_address_);
  arena_offset += size;

  return ptr;
}

} // namespace Engine::Memory

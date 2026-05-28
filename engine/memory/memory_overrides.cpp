#include "memory_overrides.hpp"

#include <cstddef>
#include <cassert>
#include <new>

void* operator new(std::size_t)
{
  assert(false && "Standard allocations are strictly forbidden!");
  throw std::bad_alloc();
}

void* operator new[](std::size_t)
{
  assert(false && "Standard array allocations are strictly forbidden!");
  throw std::bad_alloc();
}

void operator delete(void* ptr) noexcept 
{
  assert(false && "Standard deletions are stricly forbidden!");
  __builtin_unreachable();
}

void operator delete[](void* ptr) noexcept 
{
  assert(false && "Standard array deletions are strictly forbidden!");
  __builtin_unreachable();
}

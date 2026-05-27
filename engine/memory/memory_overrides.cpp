#include "memory_overrides.hpp"


void* operator new(std::size_t)
{
  assert(false && "Standard allocations are strictly forbidden!");
  throw std::bad_alloc();
}

void operator delete(void* ptr) noexcept {
  assert(false && "Standard deletions are stricly forbidden!");
  __builtin_unreachable();
}

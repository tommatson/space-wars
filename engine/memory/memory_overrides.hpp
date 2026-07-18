#pragma once

#include <cstddef>
#include <cassert>

// -----------------------------------------------------------------------------
// Core API
// -----------------------------------------------------------------------------
void* operator new (std::size_t size);
void* operator new[](std::size_t size);

void operator delete(void* ptr) noexcept;
void operator delete[](void* ptr) noexcept;

class ExternalAllocationScope{
public:
  ExternalAllocationScope();
  ~ExternalAllocationScope();
};

namespace MemoryTracker {
void report();
}

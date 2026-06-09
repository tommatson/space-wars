#include "memory_overrides.hpp"

#include <new>
#include <cstdlib>
#include <cstdio>

namespace {
  bool external_scope = false;
}

ExternalAllocationScope::ExternalAllocationScope()
{
  external_scope = true; 
}

ExternalAllocationScope::~ExternalAllocationScope()
{
  external_scope = false; 
}

namespace {
struct Stats {
  std::size_t internal_allocations = 0; 
  std::size_t external_allocations = 0; 
} stats;
}

void* operator new(std::size_t size) 
{
  if (external_scope) stats.external_allocations++;
  else stats.internal_allocations++;
  return std::malloc(size);
}

void* operator new[](std::size_t size)
{
  if (external_scope) stats.external_allocations++;
  else stats.internal_allocations++;
  return std::malloc(size);
}

void operator delete(void* ptr) noexcept
{
  if (external_scope) stats.external_allocations++;
  else stats.internal_allocations++;
  std::free(ptr);
}

void operator delete[](void* ptr) noexcept  
{
  if (external_scope) stats.external_allocations++;
  else stats.internal_allocations++;
  std::free(ptr);
}

void MemoryTracker::report()
{
  std::printf("=== Allocation Report ===\n");
  std::printf("External : %zu\n", stats.external_allocations);
  std::printf("Internal : %zu\n", stats.internal_allocations);
  if (stats.internal_allocations > 0)
      std::printf("WARNING: %zu internal allocation(s) detected!\n", stats.internal_allocations);
  std::printf("=========================\n");
}

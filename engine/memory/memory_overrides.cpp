#include "memory_overrides.hpp"

#include <new>
#include <cstdlib>
#include <cstdio>
#include <atomic>

namespace {
  thread_local int external_scope_depth = 0;
}

ExternalAllocationScope::ExternalAllocationScope()
{
  external_scope_depth++; 
}

ExternalAllocationScope::~ExternalAllocationScope()
{
  external_scope_depth--; 
}

namespace {
struct Stats {
  std::atomic<std::size_t> internal_allocations{0}; 
  std::atomic<std::size_t> external_allocations{0}; 
} stats;
}

void* operator new(std::size_t size) 
{
  if (external_scope_depth > 0) stats.external_allocations++;
  else stats.internal_allocations++;
  return std::malloc(size);
}

void* operator new[](std::size_t size)
{
  if (external_scope_depth > 0) stats.external_allocations++;
  else stats.internal_allocations++;
  return std::malloc(size);
}

void operator delete(void* ptr) noexcept
{
  if (external_scope_depth > 0) stats.external_allocations++;
  else stats.internal_allocations++;
  std::free(ptr);
}

void operator delete[](void* ptr) noexcept  
{
  if (external_scope_depth > 0) stats.external_allocations++;
  else stats.internal_allocations++;
  std::free(ptr);
}

void MemoryTracker::report()
{
  std::printf("=== Allocation Report ===\n");
  std::printf("External : %zu\n", stats.external_allocations.load());
  std::printf("Internal : %zu\n", stats.internal_allocations.load());
  if (stats.internal_allocations.load() > 0)
      std::printf("WARNING: %zu internal allocation(s) detected!\n", stats.internal_allocations.load());
  std::printf("=========================\n");
}


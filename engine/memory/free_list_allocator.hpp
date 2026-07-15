#pragma once

#include <cstddef>

namespace Engine::Memory {

class FreeListAllocator{
public:
  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  FreeListAllocator(void* ptr, std::size_t max_size) noexcept;
  ~FreeListAllocator() = default;

  // ---------------------------------------------------------------------------
  // Delete Semantics
  // ---------------------------------------------------------------------------
  FreeListAllocator(const FreeListAllocator&) = delete;
  FreeListAllocator& operator= (const FreeListAllocator&) = delete;
  FreeListAllocator(const FreeListAllocator&&) = delete;
  FreeListAllocator& operator= (const FreeListAllocator&&) = delete;
 
  // ---------------------------------------------------------------------------
  // Core API
  // ---------------------------------------------------------------------------
  [[nodiscard]] void* allocate(std::size_t size, std::size_t alignment) noexcept;
  void deallocate(void* ptr, std::size_t bytes) noexcept;

private:
  // Immutables
  const std::size_t max_size_;

  struct FreeBlock
  {
    std::size_t size; // current size
    FreeBlock* next;
  };

  struct AllocationHeader
  {
    std::size_t block_size; // max size of this block
    std::size_t padding;
  };

  // Mutables
  FreeBlock* free_block_head_;

  std::byte* try_allocate(
    FreeBlock* block,
    std::size_t alignment,
    std::size_t size,
    std::byte*& data_start,
    std::size_t& block_size) noexcept;
};

} // namespace Engine::Memory

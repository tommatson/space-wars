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
    std::size_t size;
    FreeBlock* next;
  };

  struct AllocationHeader
  {
    std::size_t block_size;
    std::size_t padding;
  };

  // Mutables
  FreeBlock* free_block_head_;

  std::byte* try_allocate(
    const FreeBlock* block,
    const std::size_t alignment,
    const std::size_t size
   ) const noexcept;

  FreeBlock* create_next_free_block(
    std::byte* free_block_start,
    std::size_t free_block_size,
    FreeBlock* free_block_next,
    std::byte* data_start,
    std::size_t data_size
   ) noexcept;
};

} // namespace Engine::Memory

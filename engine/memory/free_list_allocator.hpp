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
    std::size_t max_size;
    std::byte* data_start; // free_block_start_ + padding
    FreeBlock* next;
  };

  // Mutables
  FreeBlock* free_block_head_;


  bool try_allocate(FreeBlock* block, const std::size_t alignment, const std::size_t size) noexcept;

};

} // namespace Engine::Memory

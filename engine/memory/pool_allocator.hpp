#pragma once

#include <cstddef>

namespace Engine::Memory {

class PoolAllocator{
public:
  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  PoolAllocator(void* ptr, std::size_t max_size, std::size_t chunk_size) noexcept;

  // ---------------------------------------------------------------------------
  // Delete Semantics
  // ---------------------------------------------------------------------------
  ~PoolAllocator() = delete;
  PoolAllocator(const PoolAllocator&) = delete;
  PoolAllocator& operator= (const PoolAllocator&) = delete;
  PoolAllocator(const PoolAllocator&&) = delete;
  PoolAllocator& operator= (const PoolAllocator&&) = delete;
 
  // ---------------------------------------------------------------------------
  // Core API
  // ---------------------------------------------------------------------------
  [[nodiscard]] void* allocate(std::size_t size, std::size_t alignment) noexcept;
  void deallocate(void* ptr) noexcept;
  


private:
  // Immutables
  const std::size_t max_size_;
  const std::size_t chunk_size_;

  // Mutables
  std::byte* free_list_head_;

};

} // namespace Engine::Memory

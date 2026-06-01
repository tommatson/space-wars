#include "pool_allocator.hpp"

#include <cstddef>
#include <cassert>

namespace Engine::Memory {

PoolAllocator::PoolAllocator(void* ptr, std::size_t max_size, std::size_t chunk_size) noexcept : 
  max_size_(max_size),
  chunk_size_(chunk_size),
  free_list_head_(static_cast<std::byte*>(ptr))
{
  assert(max_size > 0 && "Pool allocator must allocate a non zero size");
  assert(chunk_size > 0 && "Pool allocator must use chunks of a non zero size");
  assert(ptr != nullptr && "Pool allocator intialized with a nullptr.");
  assert(max_size > chunk_size && "Pool allocator memory is too small for one chunk.");

  std::size_t num_chunks = max_size_ / chunk_size_; 

  std::byte* cursor = free_list_head_;

  // Iterate through all the chunks, setting their pointer to the next chunk 
  for (int i = 0; i < num_chunks; ++i)
  {
    void** current_chunk  = reinterpret_cast<void**>(cursor);

    bool isLast = (i == num_chunks - 1);
    std::byte* watermark = cursor + chunk_size_;

    *current_chunk = isLast ? nullptr : watermark;

    cursor = watermark; 
  }
}

[[nodiscard]] void* PoolAllocator::allocate() noexcept
{
  if (free_list_head_ == nullptr) [[unlikely]] return nullptr;

  void* chunk = static_cast<void*>(free_list_head_);
  void** chunk_ptr = reinterpret_cast<void**>(chunk);

  free_list_head_ = static_cast<std::byte*>(*chunk_ptr);

  return chunk;
}

void PoolAllocator::deallocate(void* ptr) noexcept 
{
  assert(ptr != nullptr && "Pool allocator cannot deallocate a nullptr.");

  void** chunk_ptr = reinterpret_cast<void**>(ptr);

  *chunk_ptr = free_list_head_;

  free_list_head_ = static_cast<std::byte*>(ptr);
}

} // namespace Engine::Memory

#include "free_list_allocator.hpp"

#include <cstddef>
#include <cassert>

namespace Engine::Memory {

FreeListAllocator::FreeListAllocator(void* ptr, std::size_t max_size) noexcept : 
  max_size_(max_size),
  free_list_head_(static_cast<std::byte*>(ptr))
{
  assert(max_size > 0 && "Free list allocator must allocate a non zero size");
  assert(ptr != nullptr && "Free list allocator intialized with a nullptr.");

  





}

[[nodiscard]] void* FreeListAllocator::allocate(std::size_t size, std::size_t alignment) noexcept
{
  if (free_list_head_ == nullptr) [[unlikely]] return nullptr;

  void* chunk = static_cast<void*>(free_list_head_);
  void** chunk_ptr = reinterpret_cast<void**>(chunk);

  free_list_head_ = static_cast<std::byte*>(*chunk_ptr);

  return chunk;
}

void FreeListAllocator::deallocate(void* ptr, std::size_t bytes) noexcept 
{
  assert(ptr != nullptr && "Pool allocator cannot deallocate a nullptr.");

  void** chunk_ptr = reinterpret_cast<void**>(ptr);

  *chunk_ptr = free_list_head_;

  free_list_head_ = static_cast<std::byte*>(ptr);
}

} // namespace Engine::Memory

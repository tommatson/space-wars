#include "free_list_allocator.hpp"

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <memory>
#include <algorithm>

namespace Engine::Memory {

// Helper function to check if the object will fit
bool FreeListAllocator::try_allocate(FreeBlock* block, const std::size_t alignment, const std::size_t size) noexcept 
{
  void* current_ptr = reinterpret_cast<void*>(reinterpret_cast<std::byte*>(block) + sizeof(FreeBlock));  

  void* aligned_ptr = std::align(std::max(alignment, alignof(FreeBlock)), size, current_ptr, block->size);

  if (aligned_ptr == nullptr) return false; 

  // Data fits
  block->data_start = reinterpret_cast<std::byte*>(aligned_ptr);
  block->size -= size;

  return true;

}

FreeListAllocator::FreeListAllocator(void* ptr, std::size_t max_size) noexcept : 
  max_size_(max_size),
  free_block_head_(reinterpret_cast<FreeBlock*>(ptr))
{
  assert(max_size >= sizeof(FreeBlock) && "Free list allocator memory is too small.");
  assert(ptr != nullptr && "Free list allocator intialized with a nullptr.");

  FreeBlock* initial_block = reinterpret_cast<FreeBlock*>(ptr);
  initial_block->size = max_size;
  initial_block->max_size = max_size;
  initial_block->next = nullptr;
  initial_block->data_start = nullptr;
}

[[nodiscard]] void* FreeListAllocator::allocate(std::size_t size, std::size_t alignment) noexcept
{
  assert(size > 0 && "Size must be greater than zero.");
  assert((alignment > 0 && (alignment & (alignment - 1)) == 0) && "Alignment must be a power of two.");

  if (free_block_head_ == nullptr) [[unlikely]] return nullptr;

  FreeBlock* prev = nullptr;
  FreeBlock* curr = free_block_head_; 

  FreeBlock* best_prev = nullptr;
  FreeBlock* best_curr = nullptr;

  // First fit
  while (curr != nullptr) 
  {
    bool allocator_fits = try_allocate(curr, alignment, size);
    if (allocator_fits) 
    {
      best_prev = prev;
      best_curr = curr;
      break;
    }
    prev = curr;
    curr = curr->next;
  }

  // No fit found
  if (best_curr == nullptr) [[unlikely]] return nullptr;

  std::size_t block_size = best_curr->size;
  std::size_t block_max_size = best_curr->max_size;
  std::byte* block_data_start = best_curr->data_start;

  if (block_size >= sizeof(FreeBlock)) 
  {
    // Split the block
    FreeBlock* next_free_block = reinterpret_cast<FreeBlock*>(block_data_start + size);

    next_free_block->size = block_max_size - block_size;
    next_free_block->next = best_curr->next;

    best_curr->max_size = size;

    if (best_prev == nullptr) 
    {
      free_block_head_ = next_free_block;
    } 
    else 
    {
      best_prev->next = next_free_block;
    }
  } 
  else 
  {
    // Do not split, consume the entire block
    if (best_prev == nullptr) 
    {
      free_block_head_ = best_curr->next;
    } 
    else 
    {
      best_prev->next = best_curr->next;
    }
  }

  return reinterpret_cast<void*>(best_curr->data_start);
}


void FreeListAllocator::deallocate(void* ptr, std::size_t bytes) noexcept 
{
  assert(ptr != nullptr && "Free list allocator cannot deallocate a nullptr.");

  std::byte* payload_start = static_cast<std::byte*>(ptr);

  // The header pointer was stashed immediately before the payload at
  // allocation time (see try_allocate), so this recovers the real header
  // regardless of how much alignment padding was inserted.
  FreeBlock* freed_block = reinterpret_cast<FreeBlock**>(payload_start)[-1];

  assert(freed_block->data_start == payload_start &&
         "Pointer does not match a live allocation from this allocator.");
  assert(freed_block->max_size == bytes &&
         "Deallocation size does not match the recorded allocation size.");

  std::byte* block_start = reinterpret_cast<std::byte*>(freed_block);
  std::size_t freed_size = freed_block->size;

  // Find insertion point: first free block at or after freed_block's address.
  FreeBlock* prev = nullptr;
  FreeBlock* curr = free_block_head_;
  while (curr != nullptr && reinterpret_cast<std::byte*>(curr) < block_start) 
  {
    prev = curr;
    curr = curr->next;
  }

  // Merge with prev, if adjacent.
  if (prev != nullptr && reinterpret_cast<std::byte*>(prev) + prev->size == block_start) 
  {
    prev->size += freed_size;
    freed_block = prev;
  } 
  else 
  {
    freed_block->size = freed_size;
    freed_block->next = curr;
    if (prev == nullptr) free_block_head_ = freed_block;
    else prev->next = freed_block;
  }

  // Merge with curr, if adjacent.
  if (curr != nullptr &&
      reinterpret_cast<std::byte*>(freed_block) + freed_block->size == reinterpret_cast<std::byte*>(curr)) 
  {
    freed_block->size += curr->size;
    freed_block->next = curr->next;
  }
}


} // namespace Engine::Memory

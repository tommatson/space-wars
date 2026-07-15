#include "free_list_allocator.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace Engine::Memory {

// Helper function to check if the object will fit
std::byte* FreeListAllocator::try_allocate(
  FreeBlock* block,
  const std::size_t alignment,
  const std::size_t size ) noexcept
{
  if (block->size < sizeof(AllocationHeader) + size) return nullptr;

  std::byte* const free_block_start = reinterpret_cast<std::byte*>(block);

  void* current_ptr = free_block_start + sizeof(AllocationHeader);
  std::size_t available_space = block->size - sizeof(AllocationHeader);

  void* aligned_ptr = std::align(std::max(alignment, alignof(AllocationHeader)), size, current_ptr, available_space);

  if (aligned_ptr == nullptr) return nullptr;

  std::byte* data_start = reinterpret_cast<std::byte*>(aligned_ptr);

  // Place the allocation header directly before the data starts
  AllocationHeader* allocation_header = reinterpret_cast<AllocationHeader*>(data_start) - 1;
  allocation_header->block_size = block->size;
  allocation_header->padding = static_cast<std::size_t>(reinterpret_cast<std::byte*>(allocation_header) - free_block_start);

  return data_start;



  std::byte* allocation_end = data_start + size;

  // The next free block must also be correctly aligned
  const std::uintptr_t end_address = reinterpret_cast<std::uintptr_t>(allocation_end);
  const std::uintptr_t aligned_end_address = (end_address + alignof(FreeBlock) - 1) & ~(alignof(FreeBlock) - 1);
  std::byte* next_free_block_start = reinterpret_cast<std::byte*>(aligned_end_address);
  std::byte* free_block_end = free_block_start + block->size;

  // Consume the remainder if it is too small to store a FreeBlock
  if (next_free_block_start > free_block_end ||
      static_cast<std::size_t>(free_block_end - next_free_block_start) < sizeof(FreeBlock))
  {
    next_free_block_start = free_block_end;
  }

  block_size = static_cast<std::size_t>(next_free_block_start - free_block_start);

  AllocationHeader* allocation_header =
    reinterpret_cast<AllocationHeader*>(data_start) - 1;
  allocation_header->block_size = block_size;
  allocation_header->padding = static_cast<std::size_t>(
    reinterpret_cast<std::byte*>(allocation_header) - free_block_start);

  return true;
}

FreeListAllocator::FreeListAllocator(void* ptr, std::size_t max_size) noexcept : 
  max_size_(max_size),
  free_block_head_(reinterpret_cast<FreeBlock*>(ptr))
{
  assert(max_size >= sizeof(FreeBlock) && "Free list allocator memory is too small.");
  assert(ptr != nullptr && "Free list allocator intialized with a nullptr.");
  assert(reinterpret_cast<std::uintptr_t>(ptr) % alignof(FreeBlock) == 0 &&
         "Free list allocator memory is not correctly aligned.");

  FreeBlock* initial_block = reinterpret_cast<FreeBlock*>(ptr);
  initial_block->size = max_size;
  initial_block->next = nullptr;
}

[[nodiscard]] void* FreeListAllocator::allocate(std::size_t size, std::size_t alignment) noexcept
{
  assert(size > 0 && "Size must be greater than zero.");
  assert((alignment > 0 && (alignment & (alignment - 1)) == 0) &&
         "Alignment must be a power of two.");

  FreeBlock* prev = nullptr;
  FreeBlock* curr = free_block_head_;

  // First fit
  while (curr != nullptr) 
  {
    std::byte* free_block = reinterpret_cast<std::byte*>(curr);

    std::size_t free_block_size = curr->size;
    FreeBlock* free_block_next = curr->next;

    std::byte* data_start = try_allocate(curr, alignment, size);
    AllocationHeader* allocation_header = reinterpret_cast<AllocationHeader*>(data_start) - 1;

    // Allocation suceeded
    if (data_start) 
    {
      // We need to sort out sizes and make space for the next FreeBlock
      free_block->size = 
      


    }

    if (try_allocate(curr, alignment, size, data_start, block_size))
    {
      std::byte* next_free_block_start = free_block_start + block_size;
      std::byte* free_block_end = free_block_start + free_block_size;

      if (next_free_block_start == free_block_end)
      {
        if (prev == nullptr) free_block_head_ = next;
        else prev->next = next;
      }
      else
      {
        FreeBlock* next_free_block = reinterpret_cast<FreeBlock*>(next_free_block_start);
        next_free_block->size = static_cast<std::size_t>(free_block_end - next_free_block_start);
        next_free_block->next = next;

        if (prev == nullptr) free_block_head_ = next_free_block;
        else prev->next = next_free_block;
      }

      return reinterpret_cast<void*>(data_start);
    }

    prev = curr;
    curr = curr->next;
  }

  return nullptr;
}

void FreeListAllocator::deallocate(void* ptr, std::size_t bytes) noexcept 
{
  if (ptr == nullptr) return;
  (void)bytes;

  std::byte* data_start = static_cast<std::byte*>(ptr);
  AllocationHeader* allocation_header =
    reinterpret_cast<AllocationHeader*>(data_start) - 1;

  std::byte* block_start =
    reinterpret_cast<std::byte*>(allocation_header) - allocation_header->padding;
  std::size_t block_size = allocation_header->block_size;
  FreeBlock* freed_block = reinterpret_cast<FreeBlock*>(block_start);

  // Find the correct position in the free list
  FreeBlock* prev = nullptr;
  FreeBlock* curr = free_block_head_;
  while (curr != nullptr && reinterpret_cast<std::byte*>(curr) < block_start) 
  {
    prev = curr;
    curr = curr->next;
  }

  // Merge with the previous free block
  if (prev != nullptr && reinterpret_cast<std::byte*>(prev) + prev->size == block_start) 
  {
    prev->size += block_size;
    freed_block = prev;
  } 
  else 
  {
    freed_block->size = block_size;
    freed_block->next = curr;

    if (prev == nullptr) free_block_head_ = freed_block;
    else prev->next = freed_block;
  }

  // Merge with the next free block
  if (curr != nullptr &&
      reinterpret_cast<std::byte*>(freed_block) + freed_block->size == reinterpret_cast<std::byte*>(curr)) 
  {
    freed_block->size += curr->size;
    freed_block->next = curr->next;
  }
}

} // namespace Engine::Memory

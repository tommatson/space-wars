#include "free_list_allocator.hpp"

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <algorithm>

namespace Engine::Memory {

struct AllocationHeader 
{
  std::size_t size;     // Total size of the block allocated (including padding)
  std::size_t padding;  // Padding from block_start to payload_start
};

// Helper function to calculate padding
inline std::size_t calculate_padding(std::uintptr_t block_start, std::size_t alignment) noexcept 
{
  const std::size_t header_size = sizeof(AllocationHeader);
  const std::size_t required_alignment = std::max(alignment, alignof(AllocationHeader));
  
  std::uintptr_t raw_payload = block_start + header_size;
  std::uintptr_t aligned_payload = (raw_payload + (required_alignment - 1)) & ~(required_alignment - 1);
  
  return aligned_payload - block_start;
}

FreeListAllocator::FreeListAllocator(void* ptr, std::size_t max_size) noexcept : 
  max_size_(max_size),
  free_block_head_(reinterpret_cast<FreeBlock*>(ptr))
{
  assert(max_size >= sizeof(FreeBlock) && "Free list allocator memory is too small.");
  assert(ptr != nullptr && "Free list allocator intialized with a nullptr.");

  FreeBlock* initial_block = reinterpret_cast<FreeBlock*>(ptr);
  initial_block->size = max_size;
  initial_block->next = nullptr;
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
  std::size_t best_padding = 0;

  // First fit
  while (curr != nullptr) 
  {
    std::size_t padding = calculate_padding(reinterpret_cast<std::uintptr_t>(curr), alignment);
    if (curr->size >= padding + size) {
      best_prev = prev;
      best_curr = curr;
      best_padding = padding;
      break;
    }
    prev = curr;
    curr = curr->next;
  }

  // No fit found
  if (best_curr == nullptr) [[unlikely]] return nullptr;

  std::size_t block_size = best_curr->size;
  std::size_t remaining_size = block_size - (best_padding + size);

  if (remaining_size >= sizeof(FreeBlock)) {
    // Split the block
    FreeBlock* next_free_block = reinterpret_cast<FreeBlock*>(
        reinterpret_cast<std::byte*>(best_curr) + best_padding + size);
    next_free_block->size = remaining_size;
    next_free_block->next = best_curr->next;

    if (best_prev == nullptr) {
      free_block_head_ = reinterpret_cast<std::byte*>(next_free_block);
    } else {
      best_prev->next = next_free_block;
    }
  } else {
    // Do not split, consume the entire block
    if (best_prev == nullptr) {
      free_block_head_ = reinterpret_cast<std::byte*>(best_curr->next);
    } else {
      best_prev->next = best_curr->next;
    }
  }

  std::byte* payload_start = reinterpret_cast<std::byte*>(best_curr) + best_padding;
  AllocationHeader* header = reinterpret_cast<AllocationHeader*>(payload_start - sizeof(AllocationHeader));
  header->padding = best_padding;
  header->size = (remaining_size >= sizeof(FreeBlock)) ? (best_padding + size) : block_size;

  return static_cast<void*>(payload_start);
}

void FreeListAllocator::deallocate(void* ptr, std::size_t bytes) noexcept 
{
  assert(ptr != nullptr && "Free list allocator cannot deallocate a nullptr.");

  std::byte* payload_start = static_cast<std::byte*>(ptr);
  AllocationHeader* header = reinterpret_cast<AllocationHeader*>(payload_start - sizeof(AllocationHeader));

  std::byte* block_start = payload_start - header->padding;
  std::size_t block_size = header->size;

  FreeBlock* freed_block = reinterpret_cast<FreeBlock*>(block_start);
  std::size_t freed_size = block_size;

  // Insert freed_block in address-sorted order
  FreeBlock* prev = nullptr;
  FreeBlock* curr = reinterpret_cast<FreeBlock*>(free_block_head_);

  while (curr != nullptr && reinterpret_cast<std::uintptr_t>(curr) < reinterpret_cast<std::uintptr_t>(freed_block)) {
    prev = curr;
    curr = curr->next;
  }

  FreeBlock* merged_prev = nullptr;

  // Try to merge with prev
  if (prev != nullptr && reinterpret_cast<std::byte*>(prev) + prev->size == block_start) {
    prev->size += freed_size;
    merged_prev = prev;
  } else {
    if (prev == nullptr) {
      free_block_head_ = reinterpret_cast<std::byte*>(freed_block);
    } else {
      prev->next = freed_block;
    }
    freed_block->size = freed_size;
    freed_block->next = curr;
    merged_prev = freed_block;
  }

  // Try to merge with curr
  if (curr != nullptr && reinterpret_cast<std::byte*>(merged_prev) + merged_prev->size == reinterpret_cast<std::byte*>(curr)) {
    merged_prev->size += curr->size;
    merged_prev->next = curr->next;
  }
}

} // namespace Engine::Memory

#include "linear_allocator.hpp"

#include <cstddef>
#include <cstdint>
#include <cassert>

namespace Engine::Memory {

LinearAllocator::LinearAllocator (void* ptr, size_t max_size) : 
  front_pointer_{static_cast<std::byte*>(ptr)}, 
  current_pointer_{static_cast<std::byte*>(ptr)}, 
  back_pointer_{static_cast<std::byte*>(ptr) + max_size} 
{
  assert(ptr != nullptr && "Allocator intialized with a nullptr.");
  assert(max_size > 0 && "Allocator must allocate a non-zero size.");
}

[[nodiscard]] void* LinearAllocator::allocate(size_t size, size_t alignment) noexcept
{
  assert(size > 0 && "Size must be greater than zero.");
  assert((alignment > 0 && (alignment & (alignment - 1)) == 0) && "Alignment must be a power of two.");

  // Calculate the address via rounding using the alignment
  const uintptr_t raw = reinterpret_cast<uintptr_t>(current_pointer_);
  const uintptr_t aligned = (raw + (alignment - 1)) & ~(alignment - 1);

  std::byte* result = reinterpret_cast<std::byte*>(aligned);
  std::byte* watermark = result + size; 

  if (watermark > back_pointer_) [[unlikely]] { 
    return nullptr;
  }

  current_pointer_ = watermark;

  return static_cast<void*>(result);
}

void LinearAllocator::clear() noexcept {
  current_pointer_ = front_pointer_;
}

} // namespace Engine::Memory


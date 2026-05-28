#pragma once

#include <cstddef>

namespace Engine::Memory {

class LinearAllocator {
public:


  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  LinearAllocator(void* ptr, std::size_t max_size);
  ~LinearAllocator() noexcept = default;

  // ---------------------------------------------------------------------------
  // Deleted Semantics
  // ---------------------------------------------------------------------------
  LinearAllocator(const LinearAllocator&) = delete;
  LinearAllocator& operator=(const LinearAllocator&) = delete;
  LinearAllocator(LinearAllocator&&) = delete;
  LinearAllocator& operator=(LinearAllocator&&) = delete;

  // ---------------------------------------------------------------------------
  // Core API
  // ---------------------------------------------------------------------------
  [[nodiscard]] void* allocate(std::size_t size, std::size_t alignment) noexcept;

  void clear() noexcept;

private:
  std::byte* const front_pointer_;
  std::byte* const back_pointer_;

  std::byte* current_pointer_;
};

} // namespace Engine::Memory


#pragma once

#include <cstddef>
#include <sys/mman.h>

namespace Engine::Memory {

class SystemArena {
public:
  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  SystemArena(size_t newSize);
  ~SystemArena();

  // ---------------------------------------------------------------------------
  // Core API
  // ---------------------------------------------------------------------------
  [[nodiscard]] void* push_memory(size_t size) noexcept;

private:
  // Mutables
  size_t total_size_ = 0;  
  size_t arena_offset = 0;
  
  // Immutables
  void* const arena_base_address_;
};

} // namespace Engine::Memory

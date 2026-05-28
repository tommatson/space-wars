#pragma once

#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <sys/mman.h>



namespace Engine { namespace Memory {

class SystemArena {
public:
  SystemArena(size_t newSize);

  ~SystemArena();

  void* push_memory(size_t size);

private:
  size_t total_size_ = 0;  

  size_t arena_offset = 0;

  void* arena_base_address_;
};

} } // namespace Engine::Memory

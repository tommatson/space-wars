#include "system_arena.hpp"


namespace Engine { namespace Memory {

SystemArena::SystemArena(size_t newSize) : total_size_(newSize){
  arena_base_address_ = mmap(
    nullptr,
    newSize,
    PROT_READ | PROT_WRITE,
    MAP_PRIVATE,
    -1,
    0
  );

  if (arena_base_address_ == MAP_FAILED) throw std::runtime_error("mmap failed!");
} 

SystemArena::~SystemArena(){
  if (arena_base_address_) munmap(arena_base_address_, total_size_);
}

void* SystemArena::push_memory(size_t size){
  if (arena_offset + size > total_size_){
    throw std::runtime_error("SystemArena is out of memory!");
  }

  void* ptr = arena_offset + static_cast<uint8_t*>(arena_base_address_);
  arena_offset += size;

  return ptr;

}

}}

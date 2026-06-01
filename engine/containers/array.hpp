#pragma once

#include <cstddef>



namespace Engine::Containers{


template<typename T, typename A>
class Array{
public:
  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  Array(A& allocator) :
  size_(0),
  capacity_(0),
  allocator_(allocator)
  {
  }
  ~Array();




  // ---------------------------------------------------------------------------
  // Core API
  // ---------------------------------------------------------------------------
  std::size_t size() noexcept;
  
  void push_back(T);
  
  void pop_back();



private:
  std::size_t size_;
  std::size_t capacity_;

  A& allocator_;
  
  T* const front_ptr_;

  void* memory_segment_;

  void reserve(std::size_t new_capacity)
  {
    if (new_capacity <= capacity_) return;

    allocator_.allocate




    
  }

};

} // namespace Engine::Containers

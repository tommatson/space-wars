#pragma once

#include <cstddef>
#include <cassert>
#include "allocator_concepts.hpp"


namespace Engine::Containers{


template<typename T, Allocator A>
class Array{
public:
  // ---------------------------------------------------------------------------
  // Leaders
  // ---------------------------------------------------------------------------
  Array(A& allocator) :
  size_(0),
  capacity_(0),
  allocator_(allocator),
  front_ptr_(nullptr)
  {
  }
  ~Array()
  {
    clear();
    
    allocator_.deallocate(reinterpret_cast<void*>(front_ptr_), capacity_ * sizeof(T));
  };

  // ---------------------------------------------------------------------------
  // Core API
  // ---------------------------------------------------------------------------
  std::size_t size() const noexcept 
  {
    return size_;
  }
  
  std::size_t capacity() const noexcept 
  {
    return capacity_;
  }
  
  void push_back(T data) 
  {
    // Check capacity
    if (size_ + 1 >= capacity_ ) [[unlikely]] reserve(capacity_ ? capacity_ * 2 : 8);

    new (front_ptr_ + size_) T(std::move(data));

    ++size_;
  }
  
  void pop_back()
  {
    assert(size_ > 0 && "Array must contain an element to pop back.");

    front_ptr_[--size_].~T();
  };


  const T& operator[](std::size_t i) const
  {
    assert(i < size_ && "Index out of range.");
    return front_ptr_[i];

  }

  void clear() 
  {
    for(std::size_t i = 0; i < size_; ++i)
    {
      front_ptr_[i].~T();
    }

    size_ = 0;
  }

  T* data() noexcept { return front_ptr_; }
  const T* data() const noexcept { return front_ptr_; }

  T* begin() noexcept { return front_ptr_; }
  const T* begin() const noexcept { return front_ptr_; }

  T* end() noexcept { return front_ptr_ + size_; }
  const T* end() const noexcept { return front_ptr_ + size_; }

private:
  std::size_t size_;
  std::size_t capacity_;

  A& allocator_;
  
  T* front_ptr_;

  void reserve(std::size_t new_capacity)
  {
    if (new_capacity <= capacity_) return;

    T* new_data  = reinterpret_cast<T*>(allocator_.allocate(new_capacity * sizeof(T), alignof(T)));

    // Move in the existing elements
    for(std::size_t i = 0; i < size_; ++i){
      new (new_data + i) T(std::move(front_ptr_[i])); 
    }

    allocator_.deallocate(reinterpret_cast<void*>(front_ptr_), capacity_ * sizeof(T));

    front_ptr_ = new_data;
    capacity_ = new_capacity;

  }

};

} // namespace Engine::Containers

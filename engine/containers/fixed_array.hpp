#pragma once

#include <cstddef>
#include <cassert>
#include <new>
#include <utility>
#include "allocator_concepts.hpp"


namespace Engine::Containers {


template<typename T, Allocator A>
class FixedArray {
public:
  FixedArray(A& allocator, std::size_t capacity) : 
  allocator_(allocator),
  size_(0),
  capacity_(capacity)
  {

  }

  ~FixedArray();


  FixedArray(const FixedArray&) = delete;
  FixedArray& operator=(const FixedArray&) = delete;
  FixedArray(FixedArray&&) = delete;
  FixedArray& operator=(FixedArray&&) = delete;


  // -----------------------------------------------------------------
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
    if (size_ == capacity_) [[unlikely]] reserve(capacity_ ? capacity_ * 2 : 8);

    new (data_ + size_) T(std::move(data));

    ++size_;
  }
  
  void pop_back()
  {
    assert(size_ > 0 && "Array must contain an element to pop back.");

    data_[--size_].~T();
  };


  const T& operator[](std::size_t i) const
  {
    assert(i < size_ && "Index out of range.");
    return data_[i];

  }

  void clear() 
  {
    for(std::size_t i = 0; i < size_; ++i)
    {
      data_[i].~T();
    }

    size_ = 0;
  }

  T* data() noexcept { return data_; }
  const T* data() const noexcept { return data_; }

  T* begin() noexcept { return data_; }
  const T* begin() const noexcept { return data_; }

  T* end() noexcept { return data_ + size_; }
  const T* end() const noexcept { return data_ + size_; }





private:
  T* data_;
  
  A& allocator_;

  std::size_t size_; 
  std::size_t capacity_;


  void reserve(std::size_t new_capacity)
  {
    if (new_capacity <= capacity_) return;

    T* new_data  = reinterpret_cast<T*>(allocator_.allocate(new_capacity * sizeof(T), alignof(T)));
    if (new_data == nullptr) [[unlikely]] throw std::bad_alloc{};

    // Move in the existing elements
    for(std::size_t i = 0; i < size_; ++i){
      new (new_data + i) T(std::move(data_[i])); 
      data_[i].~T();
    }

    if constexpr (requires { allocator_.deallocate(reinterpret_cast<void*>(data_), capacity_ * sizeof(T)); }) 
    {
      allocator_.deallocate(reinterpret_cast<void*>(data_), capacity_ * sizeof(T));
    }

    data_ = new_data;
    capacity_ = new_capacity;

  }

  





};




} // namespace Engine::Containers

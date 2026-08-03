#pragma once

#include <cstddef>
#include <cassert>
#include <new>
#include <stdexcept>
#include <utility>
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

    if constexpr (requires {
      allocator_.deallocate(reinterpret_cast<void*>(front_ptr_), capacity_ * sizeof(T));
    })
    {
      if (front_ptr_ != nullptr)
      {
        allocator_.deallocate(
          reinterpret_cast<void*>(front_ptr_), capacity_ * sizeof(T)
        );
      }
    }
  };

  Array(const Array&) = delete;
  Array& operator=(const Array&) = delete;
  Array(Array&&) = delete;
  Array& operator=(Array&&) = delete;

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
    if (size_ == capacity_) [[unlikely]] reserve(capacity_ ? capacity_ * 2 : 8);

    new (front_ptr_ + size_) T(std::move(data));

    ++size_;
  }
  
  void pop_back()
  {
    assert(size_ > 0 && "Array must contain an element to pop back.");

    front_ptr_[--size_].~T();
  };

  T& back()
  {
    assert(size_ > 0 && "Array must contain an element to access back.");
    return front_ptr_[size_ - 1];
  }

  const T& back() const
  {
    assert(size_ > 0 && "Array must contain an element to access back.");
    return front_ptr_[size_ - 1];
  }


  const T& operator[](std::size_t i) const
  {
    assert(i < size_ && "Index out of range.");
    return front_ptr_[i];

  }

  T& operator[](std::size_t i)
  {
    assert(i < size_ && "Index out of range.");
    return front_ptr_[i];
  }

  void resize(std::size_t new_size)
  {
    if (new_size < size_)
    {
      while (size_ > new_size)
      {
        front_ptr_[--size_].~T();
      }
      return;
    }

    if (new_size > capacity_) reserve(new_size);

    const std::size_t old_size = size_;
    try
    {
      while (size_ < new_size)
      {
        new (front_ptr_ + size_) T{};
        ++size_;
      }
    }
    catch (...)
    {
      while (size_ > old_size)
      {
        front_ptr_[--size_].~T();
      }
      throw;
    }
  }

  void swap_remove(std::size_t index)
  {
    assert(index < size_ && "Index out of range.");

    const std::size_t last = size_ - 1;
    if (index != last)
    {
      front_ptr_[index].~T();
      new (front_ptr_ + index) T(std::move(front_ptr_[last]));
    }

    front_ptr_[last].~T();
    --size_;
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

  void reserve(std::size_t new_capacity)
  {
    if (new_capacity <= capacity_) return;

    if constexpr (!requires {
      allocator_.deallocate(reinterpret_cast<void*>(front_ptr_), capacity_ * sizeof(T));
    })
    {
      if (front_ptr_ != nullptr)
      {
        throw std::length_error(
          "Array backed by a non-deallocating allocator cannot grow after initialization."
        );
      }
    }

    T* new_data = reinterpret_cast<T*>(
      allocator_.allocate(new_capacity * sizeof(T), alignof(T))
    );
    if (new_data == nullptr) [[unlikely]] throw std::bad_alloc{};

    for(std::size_t i = 0; i < size_; ++i)
    {
      new (new_data + i) T(std::move(front_ptr_[i]));
      front_ptr_[i].~T();
    }

    if constexpr (requires {
      allocator_.deallocate(reinterpret_cast<void*>(front_ptr_), capacity_ * sizeof(T));
    })
    {
      if (front_ptr_ != nullptr)
      {
        allocator_.deallocate(
          reinterpret_cast<void*>(front_ptr_), capacity_ * sizeof(T)
        );
      }
    }

    front_ptr_ = new_data;
    capacity_ = new_capacity;
  }

private:
  std::size_t size_;
  std::size_t capacity_;

  A& allocator_;

  T* front_ptr_;

};

} // namespace Engine::Containers

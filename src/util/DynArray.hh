#pragma once

#include <memory.h>

#include <iostream>

// The following disables clang-tidy from warning about pointer arithmetic
// Since this is a container implementation, it is unavoidable that pointer
// arithmetic is used
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)

template <typename T>
class DynArray {
 private:
  uint32_t capacity;
  uint32_t size_;
  T* data;

  // TODO: What is the point of this? We do nothing with either parameter.
  void* operator new(size_t sz, T* place) { return place; }
  // TODO: Objects containing pointers may be unable to move
  // DynArray of Strings seems to break after 2 Strings are added
  void checkGrow() {
    if (size_ >= capacity) {
      T* old = data;
      data = (T*)malloc(sizeof(T) * (capacity = capacity * 2 + 1));
      memcpy((void*)data, old, size_ * sizeof(T));
      delete[](char*) old;
    }
  }

 public:
  DynArray(uint32_t capacity)
      : capacity(capacity), size_(0), data((T*)malloc(capacity * sizeof(T))) {}
  ~DynArray() {
    for (uint32_t i = 0; i < size_; i++) data[i].~T();
    free((void*)data);
  }
  void clear() {
    for (uint32_t i = 0; i < size_; i++) data[i].~T();
    size_ = 0;
  }
  DynArray(const DynArray& orig)
      : capacity(orig.capacity),
        size_(orig.size_),
        data((T*)new char[sizeof(T) * orig.capacity]) {
    for (uint32_t i = 0; i < size_; i++) new (data + i) T(orig.data[i]);
  }

  DynArray& operator=(const DynArray& orig) = delete;
  void add(const T& v) {
    checkGrow();
    new ((void*)(data + size_++)) T(v);
  }

  T removeEnd() {
    size_--;
    T copy = data[size_];
    data[size_].~T();
    return copy;
  }
  constexpr const T& operator[](uint32_t i) const { return data[i]; }
  constexpr T& operator[](uint32_t i) { return data[i]; }
  uint32_t size() const { return size_; }
  const T& last() const { return data[size_ - 1]; }
  friend std::ostream& operator<<(std::ostream& s, const DynArray& d) {
    for (uint32_t i = 0; i < d.size_; i++) s << d.data[i] << ' ';
    return s;
  }

  constexpr T removeAt(uint32_t index) {
    T temp = data[index];

    for (int i = index; i < size_ - 1; ++i) {
      data[i] = data[i + 1];
    }
    if (index == size_ - 1) data[index].~T();
    --size_;
    return temp;
  }
  template <class... Args>
  T& emplace_back(Args&&... args) {
    checkGrow();
    return *construct_at(data + size_++, std::forward<Args>(args)...);
  }

  void push_back(T&& elem) { emplace_back(std::move(elem)); }

  constexpr bool find(T& t) const {
    for (const T& item : data) {
      if (item == t) {
        return true;
      }
    }
    return false;
  }

  // Roughly equivalent of std::construct_at. Used to reduce class footprint
  template <class... Args>
  constexpr T* construct_at(T* p, Args&&... args) {
    return ::new (const_cast<void*>(static_cast<const volatile void*>(p)))
        T(std::forward<Args>(args)...);
  }
};

// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

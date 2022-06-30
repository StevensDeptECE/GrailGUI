#pragma once

#include <memory.h>

#include <iostream>
#include <utility>
template <typename T>
class DynArray {
 private:
  uint32_t capacity;
  uint32_t size_;
  T* data;
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
    for (int i = 0; i < size_; i++) data[i].~T();
    free((void*)data);
  }
  void clear() {
    for (int i = 0; i < size_; i++) data[i].~T();
    size_ = 0;
  }
  DynArray(const DynArray& orig)
      : capacity(orig.capacity),
        size_(orig.size_),
        data((T*)malloc(sizeof(T) * orig.capacity)) {
    for (uint32_t i = 0; i < size_; i++) new (data + i) T(orig.data[i]);
  }

  DynArray& operator=(const DynArray& orig) = delete;
  void add(const T& v) {
    checkGrow();
    new ((void*)(data + size_++)) T(v);
  }

  T removeEnd() {
    size_--;
    T copy = data[size_];  // TODO: This needs to MOVE not copy
    data[size_].~T();
    return copy;
  }

  //  1 2 3 4 5
  T remove(uint32_t i) {
    T copy = data[i];  // TODO: This needs to MOVE not copy
    data[i].~T();
    for (uint32_t j = i + 1; j < size_; j++) data[j - 1] = std::move(data[j]);
    // new (data + j - 1) T(data[j]);
    // data[j].~T();
    // TODO: Alice! What the hell is this? Trying to move but the destination
    // should not be destroyed. move seems like a really bad implementation
    size_--;
    return copy;
  }
  constexpr const T& operator[](uint32_t i) const { return data[i]; }
  constexpr T& operator[](uint32_t i) { return data[i]; }
  uint32_t size() const { return size_; }
  const T& last() const { return data[size_ - 1]; }
  friend std::ostream& operator<<(std::ostream& s, const DynArray& d) {
    for (int i = 0; i < d.size_; i++) s << d.data[i] << ' ';
    return s;
  }

  constexpr bool find(T& t) {
    for (T& item : data) {
      if (item == t) {
        return true;
      }
    }
    return false;
  }
};

#pragma once

#include <iostream>
#include <memory.h>
template<typename T>
class DynArray {
private:
	uint32_t capacity;
	uint32_t size_;
	T* data;
	void* operator new(size_t sz, T* place) {
		return place;
	}
	void checkGrow() {
		if (size_ >= capacity) {
			T* old = data;
			data = (T*)malloc(sizeof(T) * (capacity = capacity*2+1));
			memcpy(data, old, size_ * sizeof(T));
			delete [] (char*)old;
		}
	}
public:
	DynArray(uint32_t capacity) : capacity(capacity), size_(0),
																data((T*)malloc(capacity * sizeof(T))) {}
	~DynArray() {
		for (int i = 0; i < size_; i++)
			data[i].~T();
		free(data);
	}
  void clear() {
		for (int i = 0; i < size_; i++)
			data[i].~T();
		size_ = 0;
	}
	DynArray(const DynArray& orig) :
		capacity(orig.capacity), size_(orig.size_),
		data((T*)new char[sizeof(T) * orig.capacity]) {
    for (uint32_t i = 0; i < size_; i++)
      new(data+i) T(orig.data[i]);
  }
  
	DynArray& operator =(const DynArray& orig) = delete;
  void add(const T& v) {
		checkGrow();
		new(data+size_++) T(v);
	}

  T removeEnd() {
		size_--;
		T copy = data[size_];
		data[size_].~T();
		return copy;
	}
	const T& operator [](uint32_t i) const { return data[i]; }
	T& operator [](uint32_t i) { return data[i]; }
	uint32_t size() const { return size_; }
  const T& last() const {
    return data[size_ - 1];
  }
	friend std::ostream& operator <<(std::ostream& s, const DynArray& d) {
		for (int i = 0; i < d.size_; i++)
			s << d.data[i] << ' ';
		return s;
	}
};

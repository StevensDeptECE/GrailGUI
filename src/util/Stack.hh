#pragma once

#include "util/DynArray.hh"
template<typename T>
class Stack {
private:
	DynArray<T> impl;
public:
	Stack(uint32_t capacity) : impl(capacity) {}
  void push(const T& v) { impl.add(v); }
  bool isEmpty() const { return impl.size() == 0; }
  T peek() const {
    return impl.last();
  }
	T pop() {
		return impl.removeEnd();
	}
	friend std::ostream& operator <<(std::ostream& s, const Stack& stack) {
		return s << stack.impl;
	}
};

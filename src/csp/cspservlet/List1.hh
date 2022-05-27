#pragma once

template <typename T>
class List1 {
 private:
  uint8_t capacity;
  uint8_t used;
  T* data;
  void checkGrow() {
    if (used < capacity) return;
    void* old = data;
    uint32_t newCapacity = 2 * capacity;
    if (newCapacity > 255) newCapacity = 255;
    // TODO: how to fail if you max out the list?
    const uint32_t numBytes = newCapacity * sizeof(T);
    void* newdata = new char[numBytes];
    data = (T*)newData;
                memcpy(data, old, newCapacity
		for (uint32_t i = 0; i < used; ++i)
			data[i] = old[i];
		delete [] old;
  }

 public:
  List1(uint8_t initialSize)
      : capacity(initialSize), used(0), data(new T[initialSize]) {}
  ~List1() { delete[] data; }
  List1(const List& orig) = delete;
  List1& operator=(const List& orig) = delete;
  void addEnd(const T& v) { checkGrow(); }
};

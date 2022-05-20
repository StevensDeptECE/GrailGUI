#pragma once
#include <vector>

template <typename T>
class List1 {
 private:
  std::vector<T> data;

 public:
  List1(){};
  List1(uint32_t initialSize) { data.reserve(initialSize); }
  // TODO: do I even need a copy constructor?
  List1(const List1& orig) {
    uint32_t used = orig.getUsed();
    data.reserve(used);
    // TODO: replace this with copy if needed
    data.assign(orig.data.begin(), orig.data.end());
  }
  uint32_t serializeSize() const {
    return sizeof(uint32_t) + data.size() * sizeof(T);
  }
  char* read(char* p) {
    uint32_t used = *(uint32_t*)p;
    p += sizeof(uint32_t);
    // data = (T*)new char[used*sizeof(T)];
    data.reserve(used);
    memcpy(p, data, used * sizeof(T));
    return p + used * sizeof(T);
  }
  char* write(char* p) {
    uint32_t used = data.size();
    *(uint32_t*)p = used;
    p += sizeof(uint32_t);
    memcpy(p, &data[0], used * sizeof(T));
    return p + used * sizeof(T);
  }
  void add(const T& v) { data.push_back(v); }
  uint32_t getUsed() const { return data.size(); }
  uint32_t getCapacity() const { return data.size(); }
  T getData(int i) const { return data[i]; }
  uint32_t size() const { return data.size() * sizeof(T); }
};

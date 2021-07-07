#pragma once

#include "xdl/std.hh"

template <typename T>
class List : public CompoundType {
 private:
  DynArray<T> impl;

 public:
  List(uint32_t size = 16) : CompoundType("LIST16"), impl(size) {}
  DataType getDataType() const { return DataType::LIST16; }
  void add(const T& e) { impl.add(e); }
#if 0
  template <class... Args>
  void add(Args&&... args) {
    impl.emplace_back(args);
  }
#endif
  uint32_t size() const override { return impl.size(); }
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  void read(Buffer& buf) {
    uint32_t len = buf._readU16();
    T val;
    for (uint32_t i = 0; i < len; i++) {
      val.read(buf);
      add(val);
    }
  }
  XDLIterator* createIterator() override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void addMeta(ArrayOfBytes* meta) const override {
    throw Ex1(Errcode::UNIMPLEMENTED);
  }
  void addData(ArrayOfBytes* data) const override {
    throw Ex1(Errcode::UNIMPLEMENTED);
  }
};

template <typename T>
void List<T>::writeMeta(Buffer& buf) const {
  buf.write(DataType::LIST16);
  if (typeToDataType(impl[0]) != DataType::UNIMPL) {
    buf.write(typeToDataType(impl[0]));
  } else {  // if (is_base_of<XDLType, T>::value) {
    impl[0].writeMeta(buf);
  }
  // else {
  // buf.write(typeid(T).name());
  //}
}

template <typename T>
void List<T>::write(Buffer& buf) const {
  buf.write(uint16_t(impl.size()));
  for (uint32_t i = 0; i < impl.size(); i++) {
    if (typeToDataType(impl[i]) != DataType::UNIMPL) {
      buf.write(impl[i]);
    } else {
      impl[i].write(buf);
    }
    buf.checkAvailableWrite();
  }
}

// TODO: implement
template <typename T>
XDLIterator* List<T>::createIterator() {
  return nullptr;
}

// TODO: Implement
template <typename T>
void List<T>::display(Buffer& binaryIn, Buffer& asciiOut) const {}

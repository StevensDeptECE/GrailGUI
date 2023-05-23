#pragma once

#include "xdl/std.hh"

/**
 * @brief A templated list of XDL Types
 *
 * Since List is templated, it is unlikely that it could be used for anything
 * more generic, so it is recommended that GenericList is used for runtime
 * generics. However, for lists that are custom written, List<T> works to store
 * anything that implements a write and writeMeta function.
 *
 * @tparam T An XDL type or a type that implements write and writeMeta
 */
template <typename T>
class List : public CompoundType {
 private:
  DynArray<T> impl;

 public:
  List(const std::string listName, uint32_t size = 16)
      : CompoundType(listName), impl(size) {}
  /**
   * @brief Get the DataType of the List
   *
   * For this list, the datatype is dynamically determined by the size of the
   * list
   *
   * @return DataType
   */
  DataType getDataType() const override {
    if (impl.size() <= UINT8_MAX) {
      return DataType::LIST8;
    } else if (impl.size() <= UINT16_MAX) {
      return DataType::LIST16;
    } else if (impl.size() <= UINT32_MAX) {
      return DataType::LIST32;
    } else if (impl.size() <= UINT64_MAX) {
      return DataType::LIST64;
    } else {
      return DataType::UNIMPL;
    }
  }

  // Consider removing the deref functions
  template <typename U>
  U deref(U* obj) {
    if (obj == nullptr) {
      throw Ex1(Errcode::BAD_ARGUMENT);
    }
    return *obj;
  }

  template <typename U>
  U deref(U& obj) {
    return obj;
  }

  /**
   * @brief Add a single element to the list
   *
   * @tparam U
   * @param e
   */
  template <typename U>
  void add(U e) {
    impl.add(deref(e));
  }

  template <class... Args>
  void add(Args&&... args) {
    (impl.add(deref(args)), ...);
  }

  uint32_t size() const override { return impl.size(); }
  uint32_t fieldSize(const List<T>& list) {
    if (list.size() > 0) return fieldSize(list[0]);
    return 1;
  }
  void read(Buffer& buf) {
    uint32_t len = buf._readU16();
    T val;
    for (uint32_t i = 0; i < len; i++) {
      val.read(buf);
      add(val);
    }
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void addMeta(ArrayOfBytes* meta) const override {
    throw Ex1(Errcode::UNIMPLEMENTED);
  }
  void addData(ArrayOfBytes* data) const override {
    throw Ex1(Errcode::UNIMPLEMENTED);
  }

  XDLType* begin(Buffer&) override;
  void writeXDL(Buffer& buf) const;
  void writeXDLMeta(Buffer& buf) const;
  const T& operator[](uint32_t i) const { return impl[i]; }
  T& operator[](uint32_t i) { return impl[i]; }
};

template <typename T>
void writeMeta(Buffer& buf, const List<T>& list) {
  buf.write(list.getDataType());
  buf.write(list.getTypeName());
  writeMeta(buf, list[0]);
}

template <typename T>
void write(Buffer& buf, const List<T>& list) {
  buf.write(uint16_t(list.size()));
  for (uint32_t i = 0; i < list.size(); i++) {
    write(buf, (list[i]));
  }
}

// TODO: Implement
template <typename T>
void List<T>::display(Buffer& binaryIn, Buffer& asciiOut) const {}

template <typename T>
XDLType* List<T>::begin(Buffer& buf) {
  return nullptr;
}  // TODO: templated lists cannot return generic pointers: &impl[0];}

template <typename T>
void List<T>::writeXDL(Buffer& buf) const {
  write(buf, *this);
}

template <typename T>
void List<T>::writeXDLMeta(Buffer& buf) const {
  writeMeta(buf, *this);
}

#include "xdl/XDLArray.hh"
// TODO: Delete class, contents should be rotated over to ArrayOfBytes in std.hh
void XDLArray::addString(const string& str) {
  add(str.length());
  for (int i = 0; i < str.length(); i++) {
    add(str[i]);
  }
}

void XDLArray::addStruct(const char name[], uint8_t numElements) {
  add((uint8_t)DataType::STRUCT8);
  addString(name);
  add(numElements);
}

void XDLArray::addBuiltinMember(DataType t, const char str[]) {
  add((uint8_t)t);
  addString(str);
}

// TODO: adding compound members: void XDLArray::addMember(XDLType* member,
// const char str[]) { member->add }
/*
        This file contains all the classes for built-in XDL types that are
        pregenerated manually as well as some example classes that will
  eventually be generated from the xdl compiler which at the moment does not
  exist.

        When that happens, this comment will change and those classes will be
        removed from std.hh

  what should be generated:
        if you declare        to send                 metadata should be
        u8 x;                 buf.write(x);           buf.write(DataType::U8);
        u16 x;                buf.write(x);           buf.write(DataType::U16);

*/
#pragma once

#include <cstdint>
#include <string>

#include "opengl/Errcode.hh"
#include "util/Buffer.hh"
#include "util/DynArray.hh"
#include "util/HashMap.hh"
#include "util/datatype.hh"

/*
  XDLType is the base class of all XDL-generated code.
  Because any information in the class will increase the runtime size of
  all children instead of storing a string typename inside XDLType, we will use
  a table At constructor time, the built-in types look up the offset in the
  table and store that (a 32-bit quantity) For a user-defined type, it
  registers the name in a HashMap in SymbolTable, and then quickly turns the
  string into an offset into the type table. This means the overhead of XDLType
  is:

          1. a pointer to the VFT since there are virtual functions
                2. the offset to the name in the array

                This is a total of 12 bytes, or 8 for a 32-bit platform.

                For speed, all methods that return a string object should return
   const string& The string must already exist so that nothing is allocated
 */

class XDLCompiler;
class Struct;
class Style;
class MultiShape2d;
class MultiText;
class XDLIterator;
class ArrayOfBytes;

class UnImpl;
class XDLType {
 protected:
  const static std::string empty;
  uint32_t nameOffset;
  static DynArray<const XDLType*> types;
  static DynArray<std::string>
      typeNames;  // the list of all unique names in the system
  static HashMap<uint32_t> byName;
  static void addType(const XDLType* type);

 public:
  const static UnImpl* unimpl;
  static void classInit();
  static void classCleanup();
  static uint32_t computeNameOffset(const std::string& typeName) {
    uint32_t nameOffset;
    if (!byName.get(typeName.c_str(), &nameOffset))
      byName.add(typeName.c_str(), nameOffset = typeNames.size());
    return nameOffset;
  }
  static uint32_t computeNameOffset(DataType t) {
    uint32_t nameOffset = 0;
    return nameOffset;
  }
  XDLType() {}
  XDLType(const std::string& typeName)
      : nameOffset(computeNameOffset(typeName)) {}
  XDLType(DataType t) : nameOffset(computeNameOffset(t)) {}
  virtual void write(Buffer& b) const = 0;
  virtual void writeMeta(Buffer& buf) const;
  virtual void addData(ArrayOfBytes* data) const;
  virtual void addMeta(ArrayOfBytes* meta) const;
  virtual uint32_t size() const = 0;
  static const XDLType* getBuiltinType(DataType dt) {
    return types[uint32_t(dt)];
  }

  virtual DataType getDataType() const = 0;
  // TODO: virtual void generateCode() = 0;
  const std::string& getTypeName() const { return typeNames[nameOffset]; }
  // using this XDL type as ASCII text, not formatted
  // each class must implement its own iterator?
  virtual XDLIterator* createIterator();
  virtual void display(Buffer& binaryIn, Buffer& asciiOut) const;
  // TODO: for efficiency, add another display function since we usually do NOT
  // want to know w,h virtual void display(Buffer& in, Canvas* c, const Style*
  // s, float x0, float y0) const; using this XDL type, output formatted
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const {}
  /* read from metadata and create a struct of all the variables specified
           if any variables are structures, then this can get recursive
   */
  static void readMeta(XDLCompiler* compiler, Buffer& in, uint32_t count,
                       Struct* s);
  static const XDLType* readMeta(XDLCompiler* compiler, Buffer& in);
  static const Struct* read(Buffer& in);
  static DataType readType(Buffer& in);
};

class XDLIterator {
 private:
  uint32_t pos;

 public:
  XDLIterator() : pos(0) {}
  void advance() { pos++; }
  void advance(uint32_t d) { pos += d; }
  uint32_t getPos() const { return pos; }
};

/*
        XDLRaw loads block binary data and can write it directly to the client
        without bothering to analyze. Useful for large datasets that don't
   change often. Uses extremely little CPU
 */
class XDLRaw : public XDLType {
 private:
  const char* data;
  size_t len;
  friend Buffer;

 public:
  XDLRaw(const char* p, size_t len) : XDLType("XDLRaw"), data(p), len(len) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class XDLBuiltinType : public XDLType {
 private:
  DataType t;

 public:
  XDLBuiltinType(const std::string& name, const DataType& t)
      : XDLType(name), t(t) {}
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const override { return t; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
};

class CompoundType : public XDLType {
 public:
  CompoundType(const std::string& name) : XDLType(name) {}
  CompoundType() : XDLType() {}
};

class U8 : public XDLBuiltinType {
 public:
  U8() : XDLBuiltinType("U8", DataType::U8) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U16 : public XDLBuiltinType {
 public:
  U16() : XDLBuiltinType("U16", DataType::U16) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U24 : public XDLBuiltinType {
 public:
  U24() : XDLBuiltinType("U24", DataType::U24) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U32 : public XDLBuiltinType {
 public:
  U32() : XDLBuiltinType("U32", DataType::U32) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U64 : public XDLBuiltinType {
 public:
  U64() : XDLBuiltinType("U64", DataType::U64) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U128 : public XDLBuiltinType {
 public:
  U128() : XDLBuiltinType("U128", DataType::U128) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U256 : public XDLBuiltinType {
 public:
  U256() : XDLBuiltinType("U256", DataType::U256) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I8 : public XDLBuiltinType {
 public:
  I8() : XDLBuiltinType("I8", DataType::I8) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I16 : public XDLBuiltinType {
 public:
  I16() : XDLBuiltinType("I16", DataType::I16) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I24 : public XDLBuiltinType {
 public:
  I24() : XDLBuiltinType("I24", DataType::I24) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I32 : public XDLBuiltinType {
 public:
  I32() : XDLBuiltinType("I32", DataType::I32) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I64 : public XDLBuiltinType {
 public:
  I64() : XDLBuiltinType("I64", DataType::I64) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I128 : public XDLBuiltinType {
 public:
  I128() : XDLBuiltinType("U128", DataType::I128) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I256 : public XDLBuiltinType {
 public:
  I256() : XDLBuiltinType("I256", DataType::I256) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Bool : public XDLBuiltinType {
 public:
  Bool() : XDLBuiltinType("BOOL", DataType::BOOL) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class F32 : public XDLBuiltinType {
 public:
  F32() : XDLBuiltinType("F32", DataType::F32) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class F64 : public XDLBuiltinType {
 public:
  F64() : XDLBuiltinType("F64", DataType::F64) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Date : public XDLBuiltinType {
 public:
  Date() : XDLBuiltinType("Date", DataType::DATE) {}
  int32_t getYear() const;
  uint32_t getMonth() const;
  uint32_t getDay() const;
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

/*
  Represent a Julian Date which uses a single double precision number to
  represent a date relative to a known "0-day" called the epoch.

  In Util is a JulianDate class that does the real Date arithmetic. That should
  be brought in here.
*/
class JulianDate : public XDLBuiltinType {
 public:
  JulianDate() : XDLBuiltinType("JulDate", DataType::JULDATE) {}
  int32_t getYear() const;
  uint32_t getMonth() const;
  uint32_t getDay() const;
  uint32_t getHour() const;
  uint32_t getMin() const;
  double getSecond() const;
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Timestamp : public XDLBuiltinType {
 public:
  Timestamp() : XDLBuiltinType("Timestamp", DataType::TIMESTAMP) {}
  void write(Buffer& b) const override;
  void writeMeta(Buffer& buf) const override;
  uint32_t size() const override;
  DataType getDataType() const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class String8 : public XDLBuiltinType {
 public:
  String8() : XDLBuiltinType("STRING8", DataType::STRING8) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class String16 : public XDLBuiltinType {
 public:
  String16() : XDLBuiltinType("STRING16", DataType::STRING16) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class String32 : public XDLBuiltinType {
 public:
  String32() : XDLBuiltinType("STRING32", DataType::STRING32) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};
class String64 : public XDLBuiltinType {
 public:
  String64() : XDLBuiltinType("STRING64", DataType::STRING64) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

// Removing example types
#if 0
class UserId : public U64 {
 private:
  std::string typeName;

 public:
  UserId(uint64_t v) : typeName("UserId"), U64(v) {}
  void writeMeta(Buffer& buf) const override;
  // TODO:    b.write(DataType::DEFINETYPE); b.write("UserId");
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Address : public String8 {
 private:
  std::string typeName;

 public:
  Address(const std::string& s) : String8(s), typeName("Address") {}
  void writeMeta(Buffer& b) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Phone : public U64 {
 private:
  std::string typeName;

 public:
  Phone(uint64_t v) : typeName("Phone"), U64(v) {}
  void writeMeta(Buffer& b) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Email : public XDLType {
 private:
  std::string val;

 public:
  Email(const std::string& s) : XDLType("Email"), val(s) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& b) const override;
  void writeMeta(Buffer& b) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class ContactInfo : public XDLType {
 private:
  Address address;
  Phone phone;
  Email email;

 public:
  ContactInfo(const Address& address, const Phone& phone, const Email& email)
      : XDLType("ContactInfo"), address(address), phone(phone), email(email) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& b) const override;
  void writeMeta(Buffer& b) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class User : public XDLType {
 private:
  UserId userid;
  string firstname;
  string lastname;
  ContactInfo contact;

 public:
  User(const UserId& userid, const char firstname[], const char lastname[],
       const Address& address, const Phone& phone, const Email& email)
      : XDLType("User"),
        userid(userid),
        firstname(firstname),
        lastname(lastname),
        contact(address, phone, email) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};
#endif

/*
  GenericList is the metadata for a list of unknown type coming in.
  A hardcoded type is more efficient
*/
class GenericList : public CompoundType {
 private:
  XDLCompiler* compiler;
  const XDLType* listType;

 public:
  GenericList(XDLCompiler* compiler, const std::string& name, DataType t)
      : CompoundType(name), compiler(compiler), listType(types[uint32_t(t)]) {}
  GenericList(XDLCompiler* compiler, const std::string& name,
              const XDLType* listType)
      : CompoundType(name), compiler(compiler), listType(listType) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  XDLIterator* createIterator() override;
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
};

template <typename T>
class List : public XDLType {
 private:
  DynArray<T> impl;

 public:
  List(uint32_t size = 16) : XDLType("LIST16"), impl(size) {}
  DataType getDataType() const { return DataType::LIST16; }
  void add(const T& e) { impl.push_back(e); }
#if 0
  template <class... Args>
  void add(Args&&... args) {
    impl.emplace_back(args);
  }
#endif
  uint32_t size() const override {
    return impl.size();  // TODO: * T.size();
  }
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
};

class TypeDef : public XDLType {
 private:
  const XDLType* type;
  bool strict;

 public:
  TypeDef(const std::string& name, const XDLType* t)
      : XDLType(name), type(t), strict(true) {}
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const { return type->getDataType(); }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Struct : public CompoundType {
 protected:
  class Member {
   public:
    uint32_t nameOffset;
    uint32_t nameLen;
    const XDLType* type;  // used only for compound types
    Member(uint32_t nameOffset, uint32_t nameLen, const XDLType* type)
        : nameOffset(nameOffset), nameLen(nameLen), type(type) {}
  };
  XDLCompiler* compiler;
  DynArray<Member> members;  // sequential list of all members
 private:
  std::string memberNames;  // single string containing names of all members

  HashMap<uint32_t> byName;  // map to index into members
  uint32_t packedSize;
  const Member& getMember(uint32_t index) const { return members[index]; }

 public:
  Struct(XDLCompiler* compiler, const std::string& name)
      : CompoundType(name), members(16), byName(16), packedSize(0) {}
  Struct(XDLCompiler* c) : Struct(c, empty) {}

  void addSym(const string& name, const XDLType* t);
  void addSymCheckNull(const string& name, const XDLType* t);
  void addSymCheckDup(const string& name, const XDLType* t);

  uint32_t addMemberName(const char memberName[]) {
    uint32_t offset = memberNames.size();
    memberNames += memberName;
    return offset;
  }
  void addMember(const std::string& name, const XDLType* t);
  void addBuiltin(const std::string& name, DataType dt);
  void addRegex(const std::string& name, const std::string& regex);
  void addTypedef(const char name[], const char type[]);
  void addStructMember(const std::string& memberName,
                       const Struct* memberStruct);

  const XDLType* getMemberType(const std::string& memberName) const {
    uint32_t index;
    return byName.get(memberName.c_str(), &index) ? members[index].type
                                                  : nullptr;
  }

  const XDLType* getMemberType(uint32_t index) const {
    return members[index].type;
  }

  uint32_t getMemberCount() const;
  std::string getMemberName(uint32_t index) const {
    const Member& m = getMember(index);
    return memberNames.substr(m.nameOffset, m.nameLen);
  }
  /*
    return the size of the object as packed bytes  (not aligned)
  */
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const { return DataType::STRUCT8; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  void addData(ArrayOfBytes* data);
  void addMeta(ArrayOfBytes* meta);
};

class Regex : public XDLType {
 private:
  std::regex rexp;

 public:
  Regex(const std::string& name, const std::string& exp)
      : XDLType(name), rexp(exp) {}
  // return true if this regex object matches the text
  bool match(const std::string& text) const { return regex_match(text, rexp); }
  uint32_t size() const override { return 0; }  // TODO: implement in .cc
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const { return DataType::REGEX; }
  // string getTypeName() const {return typeName;}
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class UnImpl : public XDLType {
 private:
 public:
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const override;
};

class ArrayOfBytes : public XDLType {
 private:
  uint8_t* currentData;
  DynArray<uint8_t> data;
  DynArray<uint8_t> metadata;

 public:
  ArrayOfBytes(uint32_t dataCapacity = 512, uint32_t metadataCapacity = 16)
      : data(dataCapacity),
        metadata(metadataCapacity),
        currentData(0),
        XDLType(DataType::STRUCT16) {}
  ArrayOfBytes(const ArrayOfBytes& orig) = delete;
  ArrayOfBytes& operator=(const ArrayOfBytes& orig) = delete;
  // TODO: this is for any primitive type NO POINTERS
  // byte order is whatever is on your machine
  // TODO: Consider using the std::is_pointer to check for pointers
  template <typename T>
  void addData(T data) {
    for (uint32_t i = 0; i < sizeof(data); i++) data.add(((char*)&data)[i]);
  }

  void addMeta(DataType t) { metadata.add(uint8_t(t)); }
  // TODO: string must be len < 256
  void addMeta(const string& s) {
    metadata.add(s.length());
    for (uint32_t i = 0; i < s.length(); i++) metadata.add(s[i]);
  }

  DynArray<uint8_t>* getMeta() { return &metadata; }
  DynArray<uint8_t>* getData() { return &data; }
};

template <typename T>
DataType typeToDataType(T arg) {
  return DataType::UNIMPL;
}

DataType typeToDataType(uint8_t) { return DataType::U8; }
DataType typeToDataType(uint16_t) { return DataType::U16; }
DataType typeToDataType(uint32_t) { return DataType::U32; }
DataType typeToDataType(uint64_t) { return DataType::U64; }
DataType typeToDataType(int8_t) { return DataType::I8; }
DataType typeToDataType(int16_t) { return DataType::I16; }
DataType typeToDataType(int32_t) { return DataType::I32; }
DataType typeToDataType(int64_t) { return DataType::I64; }
DataType typeToDataType(float) { return DataType::F32; }
DataType typeToDataType(double) { return DataType::F64; }
DataType typeToDataType(bool) { return DataType::BOOL; }
DataType typeToDataType(const std::string& str) {
  int length = str.length();
  if (length <= UINT8_MAX)
    return DataType::STRING8;
  else if (length <= UINT16_MAX)
    return DataType::STRING16;
  else if (length <= UINT32_MAX)
    return DataType::STRING32;
  else
    DataType::STRING64;
}
DataType typeToDataType(const char* str) {
  int length = strlen(str);
  if (length <= UINT8_MAX)
    return DataType::STRING8;
  else if (length <= UINT16_MAX)
    return DataType::STRING16;
  else if (length <= UINT32_MAX)
    return DataType::STRING32;
  else
    DataType::STRING64;
}

template <typename T>
void List<T>::writeMeta(Buffer& buf) const {
  buf.write(DataType::LIST16);
  if (typeToDataType(impl[0]) != DataType::UNIMPL) {
    buf.write(typeToDataType(impl[0]));
  } else if (is_base_of_v<XDLType, T>) {
    impl[0].writeMeta(buf);
  }
}

template <typename T>
void List<T>::write(Buffer& buf) const {
  buf.write(uint16_t(impl.size()));
  for (uint32_t i = 0; i < impl.size(); i++) {
    if (typeToDataType(impl[i]) != DataType::UNIMPL) {
      buf.write(impl[i]);
    } else if (is_base_of_v<XDLType, T>) {
      impl[i].write(buf);
    } else {
      throw Ex1(Errcode::UNDEFINED_TYPE);
    }
  }
}
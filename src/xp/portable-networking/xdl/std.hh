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

#include <cmath>
#include <cstdint>
#include <string>

#include "util/Errcode.hh"
#include "util/Buffer.hh"
#include "util/DynArray.hh"
#include "util/HashMap.hh"
#include "util/datatype.hh"

/*
 */

class XDLCompiler;
class Struct;
class Style;
class MultiShape2d;
class MultiText;
class XDLIterator;
class ArrayOfBytes;

class UnImpl;

/**
 * @brief XDLType is the base class of all XDL-generated code.
 *
 * Because any information in the class will increase the runtime size of
 * all children instead of storing a string typename inside XDLType, we will use
 * a table At constructor time, the built-in types look up the offset in the
 * table and store that (a 32-bit quantity) For a user-defined type, it
 * registers the name in a HashMap in SymbolTable, and then quickly turns the
 * string into an offset into the type table. This means the overhead of XDLType
 * is:
 *
 *   1. a pointer to the VFT since there are virtual functions
 *   2. the offset to the name in the array
 *
 *   This is a total of 12 bytes, or 8 for a 32-bit platform.
 *
 * For speed, all methods that return a string object should return
 * const string& The string must already exist so that nothing is allocated
 */
class XDLType {
 protected:
  const static std::string empty;
  uint32_t nameOffset;
  static DynArray<const XDLType*> types;
  static DynArray<std::string>
      typeNames;  // the list of all unique names in the system
  static HashMap<uint32_t> byName;
  static const XDLType* addType(const XDLType* type);

 public:
  const static UnImpl* unimpl;
  static void classInit();
  static void classCleanup();

  static uint32_t computeNameOffset(DataType t) {
    uint32_t nameOffset = 0;
    return nameOffset;
  }

  // Delete default constructor? we should be naming all of our XDLTypes
  XDLType() = delete;
  XDLType(const std::string& typeName) {
    if (!byName.get(typeName.c_str(), &nameOffset)) {
      // byName.checkGrow();
      byName.add(typeName.c_str(), nameOffset = typeNames.size());
      typeNames.add(typeName);
      types.add(this);
    }
  }
  XDLType(DataType t) : nameOffset(computeNameOffset(t)) {}
  virtual void writeXDL(Buffer& buf) const = 0;
  virtual void writeXDLMeta(Buffer& buf) const = 0;
  virtual void addData(ArrayOfBytes* data) const = 0;
  virtual void addMeta(ArrayOfBytes* meta) const = 0;
  virtual uint32_t size() const = 0;
  virtual XDLType* begin(Buffer& buf) = 0;
  static const XDLType* getBuiltinType(DataType dt) {
    return types[uint32_t(dt)];
  }

  virtual DataType getDataType() const = 0;
  // TODO: virtual void generateCode() = 0;
  const std::string& getTypeName() const { return typeNames[nameOffset]; }
  // using this XDL type as ASCII text, not formatted
  // each class must implement its own iterator?
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

/**
 * @brief A custom iterator superclass for XDLTypes
 *
 * Most XDL Types do not need any form of iterator to view the data, but some of
 * the container types (such as structs and lists) could use iterator types to
 * make the rendering process easier. This class is intended to be used as a
 * superclass for custom iterators for those containers, so there is a common
 * set of methods that could be used to traverse through and render each
 * datatype.
 */
class XDLIterator : public XDLType {
 protected:
  XDLType* underlying;

 public:
  XDLIterator(XDLType* underlying)
      : XDLType("Iterator"), underlying(underlying) {}
  DataType getDataType() const override { return (DataType)-1; }
  XDLType* begin(Buffer& buf) override { return nullptr; }
  XDLType* getUnderlying() const { return underlying; }
  uint32_t size() const override { return underlying->size(); }
  virtual XDLIterator* clone() = 0;
};

/**
 * @brief A XDL type for raw binary data
 *
 *
 * XDLRaw loads block binary data and can write it directly to the client
 * without bothering to analyze. Useful for large datasets that don't
 * change often. Uses extremely little CPU.
 */
class XDLRaw : public XDLType {
 private:
  const char* data;
  size_t len;
  friend Buffer;

 public:
  XDLRaw(const char* p, size_t len) : XDLType("XDLRaw"), data(p), len(len) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
  void addMeta(ArrayOfBytes* meta) const override;
  void addData(ArrayOfBytes* data) const override;
  XDLType* begin(Buffer& buf) override;
};

/**
 * @brief A superclass for primitive XDL types
 *
 * Most primitive XDL types (such as integers, floats, and strings) have a
 * common set of results for a some methods from the XDLType class. Instead of
 * reimplementing those results for each of those subclasses, this superclass
 * was written instead.
 */
class XDLBuiltinType : public XDLType {
 private:
  DataType t;

 public:
  XDLBuiltinType(const std::string& name, const DataType& t)
      : XDLType(name), t(t) {}
  DataType getDataType() const override { return t; }
  void addMeta(ArrayOfBytes* meta) const override;
  XDLType* begin(Buffer& buf) override;
};

/**
 * @brief A superclass for complex XDL types
 *
 * An XDL compound type is usually either a container for other XDL types or a
 * wrapper around another XDL type. These types are named with a unique name
 * (unlike the XDLBuiltinType, which is named its typename) and are often used
 * for data transport.
 */
class CompoundType : public XDLType {
 public:
  CompoundType(const std::string& name) : XDLType(name) {}

  // Why would we ever create a nameless compound type?
  CompoundType() = delete;
  virtual void writeXDL(Buffer& buf) const = 0;
  virtual void writeXDLMeta(Buffer& buf) const = 0;
};

/**
 * @brief A unsigned 8-bit integer
 *
 */
class U8 : public XDLBuiltinType {
 private:
  uint8_t val;

 public:
  U8(uint8_t val = 0) : XDLBuiltinType("U8", DataType::U8), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const U8& data);
  friend void writeMeta(Buffer& buf, const U8& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief An unsigned 16-bit integer
 *
 */
class U16 : public XDLBuiltinType {
 private:
  uint16_t val;

 public:
  U16(uint16_t val = 0) : XDLBuiltinType("U16", DataType::U16), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U16& a, const U16& b) { return a.val == b.val; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const U16& data);
  friend void writeMeta(Buffer& buf, const U16& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief An unsigned 24-bit integer
 *
 * The unsigned 24 bit integer was specifically designed for storing color
 * values. Any color space that stores color with three integers under 255 (RGB,
 * HSV, HSL, etc.) uses three unsigned 8-bit integers and can be stored in a
 * 24-bit integer mask.
 */
class U24 : public XDLBuiltinType {
 private:
  uint32_t val;

 public:
  U24(uint32_t val = 0)
      : XDLBuiltinType("U24", DataType::U24), val(val & 0xFFFFFF) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U24& a, const U24& b) { return a.val == b.val; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  friend void write(Buffer& buf, const U24& data);
  friend void writeMeta(Buffer& buf, const U24& data);
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief An unsigned 32-bit integer
 *
 */
class U32 : public XDLBuiltinType {
 private:
  uint32_t val;

 public:
  U32(uint32_t val = 0) : XDLBuiltinType("U32", DataType::U32), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U32& a, const U32& b) { return a.val == b.val; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const U32& data);
  friend void writeMeta(Buffer& buf, const U32& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief An unsigned 64-bit integer
 *
 */
class U64 : public XDLBuiltinType {
 private:
  uint64_t val;

 public:
  U64(uint64_t val = 0) : XDLBuiltinType("U64", DataType::U64), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U64& a, const U64& b) { return a.val == b.val; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const U64& data);
  friend void writeMeta(Buffer& buf, const U64& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief An unsigned 128-bit integer
 *
 * Since C++ does not have 128-bit integers in the STL, this is stored with two
 * unsigned 64-bit integers.
 */
class U128 : public XDLBuiltinType {
 private:
  uint64_t a, b;

 public:
  U128(uint64_t a = 0, uint64_t b = 0)
      : XDLBuiltinType("U128", DataType::U128), a(a), b(b) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U128& a, const U128& b) {
    return a.a == b.a && a.b == b.b;
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const U128& data);
  friend void writeMeta(Buffer& buf, const U128& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief An unsigned 256-bit integer
 *
 * Since C++ does not have 256-bit integers in the STL, this is stored with four
 * unsigned 64-bit integers
 */
class U256 : public XDLBuiltinType {
 private:
  uint64_t a, b, c, d;

 public:
  U256(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
      : XDLBuiltinType("U256", DataType::U256), a(a), b(b), c(c), d(d) {}
  U256() : XDLBuiltinType("U256", DataType::U256), a(0), b(0), c(0), d(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U256& a, const U256& b) {
    return a.a == b.a && a.b == b.b && a.c == b.c && a.d == b.d;
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const U256& data);
  friend void writeMeta(Buffer& buf, const U256& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A signed 8-bit integer
 *
 */
class I8 : public XDLBuiltinType {
 private:
  int8_t val;

 public:
  I8(int8_t val) : XDLBuiltinType("I8", DataType::I8), val(val) {}
  I8() : XDLBuiltinType("I8", DataType::I8), val(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const I8& data);
  friend void writeMeta(Buffer& buf, const I8& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A signed 16-bit integer
 *
 */
class I16 : public XDLBuiltinType {
 private:
  int16_t val;

 public:
  I16(int16_t val) : XDLBuiltinType("I16", DataType::I16), val(val) {}
  I16() : XDLBuiltinType("I16", DataType::I16), val(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const I16& data);
  friend void writeMeta(Buffer& buf, const I16& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A signed 24-bit integer
 *
 * Unlike its unsigned counterpart, a signed 24-bit integer is not as useful for
 * colors, as colors are not normally signed. This XDL type was primarily
 * implemented so each unsigned integer type would have a signed counterpart.
 */
class I24 : public XDLBuiltinType {
 private:
  int32_t val;

 public:
  I24(int32_t val) : XDLBuiltinType("I24", DataType::I24), val(val) {}
  I24() : XDLBuiltinType("I24", DataType::I24), val(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const I24& data);
  friend void writeMeta(Buffer& buf, const I24& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A signed 32-bit integer
 *
 */
class I32 : public XDLBuiltinType {
 private:
  int32_t val;

 public:
  I32(int32_t val) : XDLBuiltinType("I32", DataType::I32), val(val) {}
  I32() : XDLBuiltinType("I32", DataType::I32), val(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const I32& data);
  friend void writeMeta(Buffer& buf, const I32& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A signed 64-bit integer
 *
 */
class I64 : public XDLBuiltinType {
 private:
  int8_t val;

 public:
  I64(int64_t val) : XDLBuiltinType("I64", DataType::I64), val(val) {}
  I64() : XDLBuiltinType("I64", DataType::I64), val(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const I64& data);
  friend void writeMeta(Buffer& buf, const I64& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A signed 128-bit integer
 *
 * Since C++ does not have 128-bit integers in the STL as of C++ 20, this is
 * implemented using one signed 64-bit integer (to store data with the signed
 * bit) and one unsigned 64-bit integer (to store the rest of the data).
 */
class I128 : public XDLBuiltinType {
 private:
  int64_t a;
  uint64_t b;

 public:
  I128(int64_t a, uint64_t b)
      : XDLBuiltinType("I128", DataType::I128), a(a), b(b) {}
  I128(int64_t b)
      : XDLBuiltinType("I128", DataType::I128),
        a(b < 0 ? 0xFFFFFFFFFFFFFFFFLL : 0),
        b(b) {}
  I128() : XDLBuiltinType("I128", DataType::I128), a(0), b(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const I128& a, const I128& b) {
    return a.a == b.a && a.b == b.b;
  }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const I128& data);
  friend void writeMeta(Buffer& buf, const I128& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A signed 256-bit integer
 *
 * Since C++ does not have 256-bit integers in the STL as of C++ 20, this is
 * implemented using one signed 64-bit integer (to store data with the signed
 * bit) and three unsigned 64-bit integers (to store the rest of the data).
 */
class I256 : public XDLBuiltinType {
 private:
  int64_t a;
  uint64_t b, c, d;

 public:
  I256(int64_t a, uint64_t b, uint64_t c, uint64_t d)
      : XDLBuiltinType("I256", DataType::I256), a(a), b(b), c(c), d(d) {}
  I256() : XDLBuiltinType("I256", DataType::I256), a(0), b(0), c(0), d(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  friend bool operator==(const I256& a, const I256& b) {
    return a.a == b.a && a.b == b.b && a.c == b.c && a.d == b.d;
  }
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const I256& data);
  friend void writeMeta(Buffer& buf, const I256& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A Boolean type
 *
 */
class Bool : public XDLBuiltinType {
 private:
  bool val;

 public:
  Bool(bool val = false) : val(val), XDLBuiltinType("BOOL", DataType::BOOL) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
  friend void write(Buffer& buf, const Bool& data);
  friend void writeMeta(Buffer& buf, const Bool& data);
};

/**
 * @brief A single-precision floating-point number
 *
 */
class F32 : public XDLBuiltinType {
 private:
  float val;

 public:
  F32(float val = 0) : XDLBuiltinType("F32", DataType::F32), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const F32& data);
  friend void writeMeta(Buffer& buf, const F32& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A double-precision floating-point number
 *
 */
class F64 : public XDLBuiltinType {
 private:
  double val;

 public:
  F64(double val = 0) : XDLBuiltinType("F64", DataType::F64), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const F64& data);
  friend void writeMeta(Buffer& buf, const F64& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

class JulianDate;
/**
 * @brief A date type (relative to a static epoch)
 *
 * This date type, while not as precise as Julian dates, allows storage of dates
 * relative to any epoch. Although the epoch is static, this could stil mean
 * that the dates could be created relative to a calendar's start date, or
 * something similar.
 */
class Date : public XDLBuiltinType {
 private:
  int32_t date;
  static int32_t epochYear;
  Date(int32_t d) : XDLBuiltinType("Date", DataType::DATE), date(d) {}

 public:
  Date(const JulianDate& jd);
  operator JulianDate() const;
  Date() : XDLBuiltinType("Date", DataType::DATE), date(0) {}
  Date(int32_t year, uint32_t month, uint32_t day);
  Date operator+(int32_t days) const { return Date(date + days); }
  friend int32_t operator-(const Date& d1, const Date& d2) {
    return d1.date - d2.date;
  }
  Date operator-(int32_t days) const { return Date(date - days); }
  Date operator+=(int32_t days) {
    date += days;
    return *this;
  }
  Date operator-=(int32_t days) {
    date -= days;
    return *this;
  }
  bool operator==(const Date& d) const { return date == d.date; }
  // TODO: Revisit default comparison with later versions of C++
  // As of gcc 10.3.0, C++ 20 deletes the following function when
  // =default is used
  bool operator<=(const Date& d) const { return date <= d.date; }

  // Prefix increment
  Date& operator++() {
    ++date;
    return *this;  // return new value by reference
  }

  // Postfix increment
  Date operator++(int) {
    Date old = *this;  // copy old value
    operator++();      // prefix increment
    return old;        // return old value
  }

  // Prefix decrement
  Date& operator--() {
    --date;
    return *this;  // return new value by reference
  }

  // Postfix decrement
  Date operator--(int) {
    Date old = *this;  // copy old value
    operator--();      // prefix increment
    return old;        // return old value
  }

  // TODO: implement date extraction math
  int32_t getYear() const;
  uint32_t getMonth() const;
  uint32_t getDay() const;
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const Date& data);
  friend void writeMeta(Buffer& buf, const Date& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A date type that stores Julian days
 *
 * A Julian date is a count of the days since January 1st, 4173 BC on the Julian
 * calendar. Julian dates are frequently used by astronomers, and are used in
 * Grail's demos primarily for simulations of planets.
 */
class JulianDate : public XDLBuiltinType {
 public:
  friend class Date;
  static const double epoch;
  static const uint16_t daysUpTo[12];
  static const uint16_t daysInMonth[12];
  static const char* monthAbbr[12];
  static const char* monthNames[12];

 private:
  double date;
#if 0
  uint32_t Z;
  double F;
  uint32_t alpha;
  uint32_t A;
  uint32_t B;
  uint32_t C;
  uint32_t D;
  uint32_t E;
#endif
  static constexpr double invSecondsPerDay = 1.0 / 86400;
  JulianDate(double d)
      : XDLBuiltinType("JulDate", DataType::JULDATE), date(d) {}

 public:
  // ex: 1900 is NOT a leap year (divisible by 100) 1904
  // is a leap year 2000 is a leap year (% 400)
  bool static isLeap(uint32_t year) {
    return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
  }
  JulianDate(int32_t year, uint32_t month, uint32_t day, uint32_t hour = 0,
             uint32_t min = 0, uint32_t second = 0);
  JulianDate() : XDLBuiltinType("JulDate", DataType::JULDATE), date(0) {}

  JulianDate operator+(double days) const { return JulianDate(date + days); }
  friend double operator-(JulianDate a, JulianDate b) {
    return a.date - b.date;
  }
  JulianDate operator-(double days) const { return JulianDate(date - days); }
  JulianDate operator+=(double days) { return date += days; }
  JulianDate operator-=(double days) { return date -= days; }
  // TODO: Decide when two dates are equal
  // Right now 1/80 of a msec
  bool operator==(JulianDate orig) { return abs(date - orig.date) < 0.0000125; }
  operator double() const { return date; }
  // TODO: need to implement extraction of date info
  int32_t getYear() const;
  uint32_t getMonth() const;
  uint32_t getDay() const;
  uint32_t getHour() const;
  uint32_t getMinute() const;
  double getSecond() const;
  // write a Julian date into the string at dest
  void format(char dest[]) {}

  //	void format(MultiText* text, uint32_t style);
  double getJulDate() const { return date; }
  void extract(int32_t* year, uint32_t* mm, uint32_t* dd, uint32_t* hh,
               uint32_t* min, uint32_t* ss) {
    double ddate = floor(date);
    int32_t dy = ddate / 365 + ddate / 4 - ddate / 100 + ddate / 400;
    *year = epoch + dy;
    int32_t month = ddate - floor(floor(ddate / 365.2425) * 365.2425);
    *mm = month;
    *dd = 0;  // TODO: use a single lookup table for both month and day,
              // easy
    double whole = floor(date);
    double frac = date - whole;
    uint32_t h = uint32_t(frac *= 24);
    frac -= h;
    *hh = h;
    uint32_t m = uint32_t(frac *= 60);
    *min = m;
    frac -= m;
    *ss = frac * 60;
  }

  friend std::ostream& operator<<(std::ostream& s, const JulianDate& jd) {
    s << JulianDate::monthAbbr[jd.getMonth() - 1];
    return s;
  }
  friend class Date;
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const JulianDate& data);
  friend void writeMeta(Buffer& buf, const JulianDate& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

class Timestamp : public XDLBuiltinType {
 private:
  uint64_t val;

 public:
  Timestamp(uint64_t v = 0)
      : XDLBuiltinType("Timestamp", DataType::TIMESTAMP), val(v) {}
  uint32_t size() const override;
  DataType getDataType() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const Timestamp& data);
  friend void writeMeta(Buffer& buf, const Timestamp& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief An 8-bit string of characters
 *
 */
class String8 : public XDLBuiltinType {
 private:
  std::string val;

 public:
  String8(const std::string& val)
      : XDLBuiltinType("STRING8", DataType::STRING8), val(val) {}
  String8() : XDLBuiltinType("STRING8", DataType::STRING8), val(empty) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const String8& data);
  friend void writeMeta(Buffer& buf, const String8& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A 16-bit string of characters
 *
 */
class String16 : public XDLBuiltinType {
 private:
  std::string val;

 public:
  String16(const std::string& val)
      : XDLBuiltinType("STRING16", DataType::STRING16), val(val) {}
  String16() : XDLBuiltinType("STRING16", DataType::STRING16), val(empty) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const String16& data);
  friend void writeMeta(Buffer& buf, const String16& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A 32-bit string of characters
 *
 */
class String32 : public XDLBuiltinType {
 private:
  std::string val;

 public:
  String32(std::string val)
      : XDLBuiltinType("STRING32", DataType::STRING32), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const String32& data);
  friend void writeMeta(Buffer& buf, const String32& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A 64-bit string of characters
 *
 */
class String64 : public XDLBuiltinType {
 private:
  std::string val;

 public:
  String64(std::string val)
      : XDLBuiltinType("STRING64", DataType::STRING64), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  friend void write(Buffer& buf, const String64& data);
  friend void writeMeta(Buffer& buf, const String64& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/*
  GenericList is the metadata for a list of unknown type coming in.
  A hardcoded type is more efficient
*/

/**
 * @brief A generic list of metadata
 *
 * This XDL container type allows a user to send and receive lists of data of
 * any type. This is particularly useful for dealing with data created or
 * manipulated by a user. When combined with the templated List type in list.hh,
 * a server could send the templated list and the client could receive the data
 * as a generic list (meaning significant performance boost on the server side).
 */
class GenericList : public CompoundType {
 private:
  XDLCompiler* compiler;
  const XDLType* listType;
  // uint32_t size_;
  // uint32_t capacity_;

 public:
  GenericList(XDLCompiler* compiler, const std::string& name, DataType t)
      : CompoundType(name), compiler(compiler), listType(types[uint32_t(t)]) {}
  GenericList(XDLCompiler* compiler, const std::string& name,
              const XDLType* listType)
      : CompoundType(name), compiler(compiler), listType(listType) {}
  XDLType* getListType() const { return (XDLType*)listType; }
  DataType getDataType() const override;
  uint32_t size() const override;
  XDLType* begin(Buffer& buf) override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
  friend void write(Buffer& buf, const GenericList& data);
  friend void writeMeta(Buffer& buf, const GenericList& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
  class Iterator : public XDLIterator {
   private:
    GenericList* list;
    Buffer& buf;
    uint32_t remaining;

   public:
    Iterator(GenericList* list, Buffer& buf)
        : XDLIterator(list), list(list), buf(buf) {
      remaining = buf.readU16();
    }
    bool operator!() const { return remaining > 0; }
    XDLType* getListType() { return list->getListType(); }
#if 0
        bool operator != (const Iterator & other) {
          return remaining > 0;
        }
#endif
    Iterator& operator++() {
      remaining--;
      return *this;
    }

    void writeXDL(Buffer& buf) const override;
    void writeXDLMeta(Buffer& buf) const override;
    void addData(ArrayOfBytes* data) const override {}
    void addMeta(ArrayOfBytes* meta) const override {}
    XDLIterator* clone() override { return new Iterator(*this); }
  };
};

#if 0
//TODO: There is already an XDLBuiltinType. Ensure that nothing uses this and then delete it.
class BuiltinType : public XDLType {
 private:
  DataType t;

 public:
  BuiltinType(const std::string& name, const DataType& t)
      : XDLType(name), t(t) {}
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const override { return t; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  XDLType* begin(Buffer& buf) override;
};
#endif

/**
 * @brief A wrapper type that allows XDL types to be defined as different types
 *
 */
class TypeDef : public CompoundType {
 private:
  const XDLType* type;
  bool strict;

 public:
  TypeDef(const std::string& name, const XDLType* t)
      : CompoundType(name), type(t), strict(true) {}
  uint32_t size() const override;
  DataType getDataType() const override { return type->getDataType(); }
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  XDLType* begin(Buffer& buf) override { return ((XDLType*)type)->begin(buf); }
  friend void write(Buffer& buf, const TypeDef& data);
  friend void writeMeta(Buffer& buf, const TypeDef& data);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

/**
 * @brief A structure of data, similar to C-style structs
 *
 * XDL structs allow multiple of any XDL type to be added to themselves and then
 * sent to a client as one package of known datatypes.
 */
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

 public:
  Struct(XDLCompiler* compiler, const std::string& name)
      : CompoundType(name), members(16), byName(16), packedSize(0) {}
  Struct(XDLCompiler* c) : Struct(c, empty) {}

  void addSym(const std::string& name, const XDLType* t);
  void addSymCheckNull(const std::string& name, const XDLType* t2);
  void addSymCheckDup(const std::string& name, const XDLType* t2);

  uint32_t addMemberName(const char memberName[]) {
    uint32_t offset = memberNames.size();
    memberNames += memberName;
    return offset;
  }
  void addMember(const std::string& name, const XDLType* t);
  void addBuiltin(const std::string& name, DataType dt);
  // void addRegex(const std::string& name, const std::string& regex);
  void addTypedef(const char name[], const char type[]);
  void addStructMember(const std::string& memberName,
                       const Struct* memberStruct);

  const Member& getMember(uint32_t index) const { return members[index]; }
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
  DataType getDataType() const override { return DataType::STRUCT8; }
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
  XDLType* begin(Buffer& buf) override { return new Iterator(this); }
  friend void write(Buffer& buf, const Struct& s);
  friend void writeMeta(Buffer& buf, const Struct& s);
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;

  /**
   * @brief An implementation of XDLIterator for structs
   *
   * This makes it easier to iterate through a struct and render its
   * contents
   */
  class Iterator : public XDLIterator {
   private:
    uint32_t currentField;

   public:
    Iterator(Struct* s) : XDLIterator(s), currentField(0) {}
    Struct* getStruct() { return (Struct*)underlying; }
    const XDLType* operator*() const {
      return ((Struct*)underlying)->getMemberType(currentField);
    }
    bool operator!() const {
      return currentField < ((Struct*)underlying)->getMemberCount();
    }
    Iterator& operator++() {
      currentField++;
      return *this;
    }

    void writeXDL(Buffer& buf) const override;
    void writeXDLMeta(Buffer& buf) const override;
    void addData(ArrayOfBytes* data) const override {}
    void addMeta(ArrayOfBytes* meta) const override {}
    XDLIterator* clone() override { return new Iterator(*this); }
  };
};

// Currently removed, not used in anything
#if 0
/**
 * @brief A wrapper on C++'s regex type for XDL
 *
 */
class Regex : public CompoundType {
 private:
  std::regex rexp;

 public:
  Regex(const std::string& name, const std::string& exp);
  // return true if this regex object matches the text
  bool match(const std::string& text) const { return regex_match(text, rexp); }
  uint32_t size() const override;
  DataType getDataType() const override;
  // string getTypeName() const {return typeName;}
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
  XDLType* begin(Buffer& buf) override;
};
#endif

/**
 * @brief An XDL type for unimplemented types
 *
 * This allows operations to fail out because a required type was never
 * implemented, or because the behavior is not allowed or undefined. This type
 * is most similar to a null pointer in other languages.
 */
class UnImpl : public XDLType {
 private:
 public:
  UnImpl() : XDLType("UnImpl") {}
  uint32_t size() const override;
  DataType getDataType() const override;
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
  XDLType* begin(Buffer& buf) override { return nullptr; }
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
};

// TODO: Add leap years
/**
 * @brief A calendar of dates
 *
 */
class Calendar : public CompoundType {
 private:
  std::string calendarName;  // name of this calendar
  Date start;
  uint32_t weekendDays : 7;
  // Index into the holiday table for each year in the calendar
  std::vector<Date> holidays;  // offset for each holiday in a given year
 public:
  Calendar(const std::string& name)
      : CompoundType("Calendar"), calendarName(name) {}
  void save(const char filename[]) const;
  void setHoliday(const Date& d);
  void setHoliday(const Date& from, const Date& to);
  void setRepeatingHoliday(int32_t yearStart, int32_t yearEnd, uint32_t month,
                           uint32_t day);
  bool isHoliday(const Date& d) const;

  Date addBusinessDays(const Date& d, int32_t delta) const;
  int32_t businessDaysBetween(const Date& d1, const Date& d2) const;

  uint32_t size() const override;
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
  void writeXDL(Buffer& buf) const override;
  void writeXDLMeta(Buffer& buf) const override;
  XDLType* begin(Buffer& buf) override;
  DataType getDataType() const override{return DataType::STRUCT8; }
};

/**
 * @brief A container for storing both raw data and metadata
 *
 * The array of bytes stores data and metadata in separate DynArrays as raw
 * binary data. This makes sending the data and metadata as easy as writing
 * whichever is requested directly to the buffer. Similarly, the client could
 * read from the buffer into an ArrayOfBytes, parse out the types from the
 * metadata, and then repeatedly iterate through the data.
 */
class ArrayOfBytes : public CompoundType {
 private:
  uint8_t* currentData;
  DynArray<uint8_t> data;
  DynArray<uint8_t> metadata;

 public:
  ArrayOfBytes(uint32_t dataCapacity = 512, uint32_t metadataCapacity = 16)
      : data(dataCapacity),
        metadata(metadataCapacity),
        currentData(0),
        CompoundType("ArrayOfBytes") {}
  ArrayOfBytes(const ArrayOfBytes& orig) = delete;
  ArrayOfBytes& operator=(const ArrayOfBytes& orig) = delete;
  // TODO: this is for any primitive type NO POINTERS
  // byte order is whatever is on your machine
  // TODO: Consider using the std::is_pointer to check for pointers
  template <typename T>
  void addData(T elem) {
    for (uint32_t i = 0; i < sizeof(elem); i++) data.add(((char*)&elem)[i]);
  }

  void addMeta(DataType t) { metadata.add(uint8_t(t)); }
  // TODO: string must be len < 256
  void addMeta(const std::string& s) {
    metadata.add(s.length());
    for (uint32_t i = 0; i < s.length(); i++) metadata.add(s[i]);
  }

  DynArray<uint8_t>* getMeta() { return &metadata; }
  DynArray<uint8_t>* getData() { return &data; }

  uint32_t size() const override;
  DataType getDataType() const override;

  void addData(ArrayOfBytes* data) const override {}
  void addMeta(ArrayOfBytes* meta) const override {}
  void writeXDL(Buffer& out) const override;
  void writeXDLMeta(Buffer& out) const override;
  friend void write(Buffer& out, const ArrayOfBytes& data);
  friend void writeMeta(Buffer& out, const ArrayOfBytes& data);
  void addStruct(const char name[], uint8_t numElements);
  void addBuiltinMember(DataType t, const char str[]);
  XDLType* begin(Buffer& buf) override { return this; }
};

/**
 * @brief Convert C++ types to XDL DataTypes
 *
 * @return DataType
 */
constexpr DataType typeToDataType(uint8_t) { return DataType::U8; }
constexpr DataType typeToDataType(uint16_t) { return DataType::U16; }
constexpr DataType typeToDataType(uint32_t) { return DataType::U32; }
constexpr DataType typeToDataType(uint64_t) { return DataType::U64; }
constexpr DataType typeToDataType(int8_t) { return DataType::I8; }
constexpr DataType typeToDataType(int16_t) { return DataType::I16; }
constexpr DataType typeToDataType(int32_t) { return DataType::I32; }
constexpr DataType typeToDataType(int64_t) { return DataType::I64; }
constexpr DataType typeToDataType(float) { return DataType::F32; }
constexpr DataType typeToDataType(double) { return DataType::F64; }
constexpr DataType typeToDataType(bool) { return DataType::BOOL; }

// constexpr lengths for std::strings unimplemented as of GCC 11.1.0
inline DataType typeToDataType(const std::string& str) {
  int length = std::char_traits<char>::length(str.c_str());
  if (length <= UINT8_MAX)
    return DataType::STRING8;
  else if (length <= UINT16_MAX)
    return DataType::STRING16;
  else if (length <= UINT32_MAX)
    return DataType::STRING32;
  else
    return DataType::STRING64;
}

constexpr DataType typeToDataType(const char* str) {
  int length = std::char_traits<char>::length(str);
  if (length <= UINT8_MAX)
    return DataType::STRING8;
  else if (length <= UINT16_MAX)
    return DataType::STRING16;
  else if (length <= UINT32_MAX)
    return DataType::STRING32;
  else
    return DataType::STRING64;
}

template <typename T>
constexpr DataType typeToDataType(T arg) {
  return DataType::UNIMPL;
}

// TODO: restrict T to builtin types?
template <typename T>
void write(Buffer& buf, T val) {
  buf.write(val);
}
// void writeMeta(Buffer& buf, uint32_t) { buf.write(DataType::U32); }
// void writeMeta(Buffer& buf, uint8_t) { buf.write(DataType::U32); }

inline void writeMeta(Buffer& buf, const U8&) { buf.write(DataType::U8); }
inline void writeMeta(Buffer& buf, const U16&) { buf.write(DataType::U16); }
inline void writeMeta(Buffer& buf, const U24&) { buf.write(DataType::U24); }
inline void writeMeta(Buffer& buf, const U32&) { buf.write(DataType::U32); }
inline void writeMeta(Buffer& buf, const U64&) { buf.write(DataType::U64); }
inline void writeMeta(Buffer& buf, const U128&) { buf.write(DataType::U128); }
inline void writeMeta(Buffer& buf, const U256&) { buf.write(DataType::U256); }
inline void writeMeta(Buffer& buf, const I8&) { buf.write(DataType::I8); }
inline void writeMeta(Buffer& buf, const I16&) { buf.write(DataType::I16); }
inline void writeMeta(Buffer& buf, const I24&) { buf.write(DataType::I24); }
inline void writeMeta(Buffer& buf, const I32&) { buf.write(DataType::I32); }
inline void writeMeta(Buffer& buf, const I64&) { buf.write(DataType::I64); }
inline void writeMeta(Buffer& buf, const I128&) { buf.write(DataType::I128); }
inline void writeMeta(Buffer& buf, const I256&) { buf.write(DataType::I256); }
inline void writeMeta(Buffer& buf, const F32&) { buf.write(DataType::F32); }
inline void writeMeta(Buffer& buf, const F64&) { buf.write(DataType::F64); }
inline void writeMeta(Buffer& buf, const Bool&) { buf.write(DataType::BOOL); }
inline void writeMeta(Buffer& buf, const String8&) {
  buf.write(DataType::STRING8);
}
inline void writeMeta(Buffer& buf, const String16&) {
  buf.write(DataType::STRING16);
}
inline void writeMeta(Buffer& buf, const String32&) {
  buf.write(DataType::STRING32);
}
inline void writeMeta(Buffer& buf, const String64&) {
  buf.write(DataType::STRING64);
}
inline void writeMeta(Buffer& buf, const Date&) { buf.write(DataType::DATE); }
inline void writeMeta(Buffer& buf, const JulianDate&) {
  buf.write(DataType::JULDATE);
}

template <typename T>
void writeMeta(Buffer& buf, const T& data) {
  buf.write(typeToDataType(data));
}
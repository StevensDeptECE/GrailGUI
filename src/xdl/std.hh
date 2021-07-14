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
      byName.checkGrow();
      byName.add(typeName.c_str(), nameOffset = typeNames.size());
      typeNames.add(typeName);
      types.add(this);
    }
  }
  XDLType(DataType t) : nameOffset(computeNameOffset(t)) {}
  virtual void write(Buffer& b) const = 0;
  virtual void writeMeta(Buffer& buf) const;
  virtual void addData(ArrayOfBytes* data) const = 0;
  virtual void addMeta(ArrayOfBytes* meta) const = 0;
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
  void addMeta(ArrayOfBytes* meta) const override;
  void addData(ArrayOfBytes* data) const override;
};

class XDLBuiltinType : public XDLType {
 private:
  DataType t;

 public:
  XDLBuiltinType(const std::string& name, const DataType& t)
      : XDLType(name), t(t) {}
  DataType getDataType() const override { return t; }
  void addMeta(ArrayOfBytes* meta) const override;
};

class CompoundType : public XDLType {
 public:
  CompoundType(const std::string& name) : XDLType(name) {}

  // Why would we ever create a nameless compound type?
  CompoundType() = delete;
  virtual void write(Buffer& buf) const = 0;
  virtual void writeMeta(Buffer& buf) const = 0;
};

class U8 : public XDLBuiltinType {
 private:
  uint8_t val;

 public:
  U8(uint8_t val = 0) : XDLBuiltinType("U8", DataType::U8), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const override;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U16 : public XDLBuiltinType {
 private:
  uint16_t val;

 public:
  U16(uint16_t val = 0) : XDLBuiltinType("U16", DataType::U16), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U16& a, const U16& b) = default;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U24 : public XDLBuiltinType {
 private:
  uint32_t val;

 public:
  U24(uint32_t val = 0)
      : XDLBuiltinType("U24", DataType::U24), val(val & 0xFFFFFF) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U24& a, const U24& b) = default;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U32 : public XDLBuiltinType {
 private:
  uint32_t val;

 public:
  U32(uint32_t val = 0) : XDLBuiltinType("U32", DataType::U32), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U32& a, const U32& b) = default;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U64 : public XDLBuiltinType {
 private:
  uint64_t val;

 public:
  U64(uint64_t val = 0) : XDLBuiltinType("U64", DataType::U64), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U64& a, const U64& b) = default;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U128 : public XDLBuiltinType {
 private:
  uint64_t a, b;

 public:
  U128(uint64_t a = 0, uint64_t b = 0)
      : XDLBuiltinType("U128", DataType::U128), a(a), b(b) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U128& a, const U128& b) = default;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class U256 : public XDLBuiltinType {
 private:
  uint64_t a, b, c, d;

 public:
  U256(uint64_t a, uint64_t b, uint64_t c, uint64_t d)
      : XDLBuiltinType("U256", DataType::U256), a(a), b(b), c(c), d(d) {}
  U256() : XDLBuiltinType("U256", DataType::U256), a(0), b(0), c(0), d(0) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const U256& a, const U256& b) = default;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I8 : public XDLBuiltinType {
 private:
  int8_t val;

 public:
  I8(int8_t val) : XDLBuiltinType("I8", DataType::I8), val(val) {}
  I8() : XDLBuiltinType("I8", DataType::I8), val(0) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I16 : public XDLBuiltinType {
 private:
  int16_t val;

 public:
  I16(int16_t val) : XDLBuiltinType("I16", DataType::I16), val(val) {}
  I16() : XDLBuiltinType("I16", DataType::I16), val(0) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I24 : public XDLBuiltinType {
 private:
  int32_t val;

 public:
  I24(int32_t val) : XDLBuiltinType("I24", DataType::I24), val(val) {}
  I24() : XDLBuiltinType("I24", DataType::I24), val(0) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I32 : public XDLBuiltinType {
 private:
  int32_t val;

 public:
  I32(int32_t val) : XDLBuiltinType("I32", DataType::I32), val(val) {}
  I32() : XDLBuiltinType("I32", DataType::I32), val(0) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class I64 : public XDLBuiltinType {
 private:
  int8_t val;

 public:
  I64(int64_t val) : XDLBuiltinType("I64", DataType::I64), val(val) {}
  I64() : XDLBuiltinType("I64", DataType::I64), val(0) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

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
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  friend bool operator==(const I128& a, const I128& b) = default;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

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
  friend bool operator==(const I256& a, const I256& b) = default;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Bool : public XDLBuiltinType {
 private:
  bool val;

 public:
  Bool(bool val = false) : val(val), XDLBuiltinType("BOOL", DataType::BOOL) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class F32 : public XDLBuiltinType {
 private:
  float val;

 public:
  F32(float val = 0) : XDLBuiltinType("F32", DataType::F32), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class F64 : public XDLBuiltinType {
 private:
  double val;

 public:
  F64(double val = 0) : XDLBuiltinType("F64", DataType::F64), val(val) {}
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class JulianDate;
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
  bool operator==(const Date& d) const = default;
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
  DataType getDataType() const;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

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
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class Timestamp : public XDLBuiltinType {
 private:
  uint64_t val;

 public:
  Timestamp(uint64_t v = 0)
      : XDLBuiltinType("Timestamp", DataType::TIMESTAMP), val(v) {}
  uint32_t size() const override;
  DataType getDataType() const override;
  void write(Buffer& b) const override;
  void writeMeta(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class String8 : public XDLBuiltinType {
 private:
  std::string val;

 public:
  String8(const std::string& val)
      : XDLBuiltinType("STRING8", DataType::STRING8), val(val) {}
  String8() : XDLBuiltinType("STRING8", DataType::STRING8), val(empty) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class String16 : public XDLBuiltinType {
 private:
  std::string val;

 public:
  String16(const std::string& val)
      : XDLBuiltinType("STRING16", DataType::STRING16), val(val) {}
  String16() : XDLBuiltinType("STRING16", DataType::STRING16), val(empty) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class String32 : public XDLBuiltinType {
 private:
  std::string val;

 public:
  String32(string val)
      : XDLBuiltinType("STRING32", DataType::STRING32), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class String64 : public XDLBuiltinType {
 private:
  std::string val;

 public:
  String64(string val)
      : XDLBuiltinType("STRING64", DataType::STRING64), val(val) {}
  DataType getDataType() const override;
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void addData(ArrayOfBytes* data) const override;
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
  uint32_t size_;
  uint32_t capacity_;

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
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class TypeDef : public CompoundType {
 private:
  const XDLType* type;
  bool strict;

 public:
  TypeDef(const std::string& name, const XDLType* t)
      : CompoundType(name), type(t), strict(true) {}
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const { return type->getDataType(); }
  void addData(ArrayOfBytes* data) const;
  void addMeta(ArrayOfBytes* meta) const;
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
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
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
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
  void display(Buffer& binaryIn, Buffer& asciiOut) const;
  void format(Buffer& binaryIn, Buffer& asciiOut, const char fmt[]) const;
};

class UnImpl : public XDLType {
 private:
 public:
  UnImpl() : XDLType("UnImpl") {}
  uint32_t size() const override;
  void write(Buffer& buf) const override;
  void writeMeta(Buffer& buf) const override;
  DataType getDataType() const override;
  void addData(ArrayOfBytes* data) const override;
  void addMeta(ArrayOfBytes* meta) const override;
};

// TODO: Add leap years
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

  void writeMeta(Buffer& b);
  void write(Buffer& b);

  void addData(ArrayOfBytes* data) const;
  void addMeta(ArrayOfBytes* meta) const;
};

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
  void writeMeta(Buffer& buf) const override;
  template <typename T>
  void addData(T elem) {
    for (uint32_t i = 0; i < sizeof(elem); i++) data.add(((char*)&elem)[i]);
  }

  void addMeta(DataType t) { metadata.add(uint8_t(t)); }
  // TODO: string must be len < 256
  void addMeta(const string& s) {
    metadata.add(s.length());
    for (uint32_t i = 0; i < s.length(); i++) metadata.add(s[i]);
  }

  DynArray<uint8_t>* getMeta() { return &metadata; }
  DynArray<uint8_t>* getData() { return &data; }

  void write(Buffer& b) const override;
  uint32_t size() const override;
  DataType getDataType() const override;

  void addData(ArrayOfBytes* data) const;
  void addMeta(ArrayOfBytes* meta) const;
  void addStruct(const char name[], uint8_t numElements);
  void addBuiltinMember(DataType t, const char str[]);
};

inline DataType typeToDataType(uint8_t) { return DataType::U8; }
inline DataType typeToDataType(uint16_t) { return DataType::U16; }
inline DataType typeToDataType(uint32_t) { return DataType::U32; }
inline DataType typeToDataType(uint64_t) { return DataType::U64; }
inline DataType typeToDataType(int8_t) { return DataType::I8; }
inline DataType typeToDataType(int16_t) { return DataType::I16; }
inline DataType typeToDataType(int32_t) { return DataType::I32; }
inline DataType typeToDataType(int64_t) { return DataType::I64; }
inline DataType typeToDataType(float) { return DataType::F32; }
inline DataType typeToDataType(double) { return DataType::F64; }
inline DataType typeToDataType(bool) { return DataType::BOOL; }
inline DataType typeToDataType(const std::string& str) {
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
inline DataType typeToDataType(const char* str) {
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
inline DataType typeToDataType(T arg) {
  return DataType::UNIMPL;
}

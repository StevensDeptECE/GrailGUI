#pragma once

#include <cstdarg>
#include <cstdint>
#include <fstream>
#include <map>
#include <regex>
#include <string>
//#include <boost/any.hpp>

// using namespace boost;

/**
         Represent a config file used to configure complicated applications

         File format supports comments starting with #
         name-value pairs of data, with declaration of the type of names
   supported in the constructor so that subclsses of Config can specify the
   required types of each tag.  In this way, the Config utility can display the
         line number and an error if the data is not as required

         TODO: Since Config files can be very large, and parsing is slow, it is
         possible to extend this model to automatically generate a binary
   version of the config.  In this way, the config file can effectively be
   compiled. This is important only if the file is large so at the moment we can
   ignore
 */

class BadType {  // BadType exception is thrown when an incorrect type is passed
                 // as a parameter for the "get" functions
 private:
  const std::string filename;
  int lineNum;

 public:
  BadType(const char filename[], int lineNum)
      : filename(filename), lineNum(lineNum) {}
  friend std::ostream& operator<<(std::ostream& s, const BadType& e) {
    return s << "Bad Type at " << e.filename << ": " << e.lineNum;
  }
};

class Config {
 public:
  // TODO: this should use the Datatype types
  enum class Type { U16, U32, U64, I16, I32, I64, F64, F32, BOOL, STRING };

 private:
  struct Sym {
    Type type;
    union {
      uint16_t u16;
      uint32_t u32;
      uint64_t u64;
      int16_t i16;
      int32_t i32;
      int64_t i64;
      double d;
      std::string* s;
      bool b;
    };

    // Constructors
    Sym(Type t, const std::string& val) : type(t) {
      // TODO: check online for actual integer bit-precision conversions
      if (t == Type::U16)
        u16 = std::stoi(val);
      else if (t == Type::U32)
        u32 = std::stoi(val);
      else if (t == Type::U64)
        u64 = std::stoi(val);
      else if (t == Type::I16)
        i16 = std::stoi(val);
      else if (t == Type::I32)
        i32 = std::stoi(val);
      else if (t == Type::I64)
        i64 = std::stoi(val);
      else if (t == Type::F64)
        d = std::stod(val);
      else if (t == Type::STRING)
        s = new std::string(val);
      else if (t == Type::BOOL)
        b = std::stoi(val);
      else
        throw BadType(__FILE__, __LINE__);
    }
    Sym(uint16_t u16) : type(Type::U16), u16(u16) {}
    Sym(uint32_t u32) : type(Type::U32), u32(u32) {}
    Sym(uint64_t u64) : type(Type::U64), u64(u64) {}
    Sym(int16_t i16) : type(Type::I16), i16(i16) {}
    Sym(int32_t i32) : type(Type::I32), i32(i32) {}
    Sym(int64_t i64) : type(Type::I64), i64(i64) {}
    Sym(double d) : type(Type::F64), d(d) {}
    Sym(const std::string& str) : type(Type::STRING), s(new std::string(str)) {}
    Sym(bool b) : type(Type::BOOL), b(b) {}         // true = 1, false = 0
    Sym(double size, char mul) : type(Type::U64) {  // 1024 or 128k or 16M or 2G
      if (mul == 'k') size *= 1024;
      if (mul == 'M') size *= 1024 * 1024;
      if (mul == 'G') size *= 1024 * 1024 * 1024;
      u64 = uint64_t(size);
    }
    Sym(const Sym& orig) {
      memcpy(this, &orig, sizeof(*this));
      if (type == Type::STRING) {
        s = new std::string(orig.s->c_str());
      }
    }
    // void operator =(const Sym& orig) {
    // 	if (this != &orig) {
    // 		~Sym();
    // 		type = orig.type;
    // 		s = new string(orig.s->c_str());
    // 	}
    // 	return *this;
    // }
    Sym& operator=(Sym orig) {
      std::swap(*this, orig);
      return *this;
    }
    ~Sym() {
      switch (type) {
        // TODO: there is an issue with the case for STRING, had to comment it
        // out to make configTester run case STRING: delete [] s; break;
        default:;
      }
    } //BUG: clang-tidy is picking up a memory leak here
  };
  std::map<std::string, Type> types;
  std::map<std::string, Type> optTypes;
  std::map<std::string, Sym> f;

  const static std::regex commentOrWhitespace;
  const static std::regex checkVector;

  const Sym* getSym(const std::string& name, int linenum) const {
    std::map<std::string, Sym>::const_iterator i = f.find(name);
    if (i == f.end()) throw BadType(__FILE__, linenum);
    return &i->second;
  }

 public:
  Config() {}
  void load(const std::string& filename);
  void save(const std::string& filename);

  template <typename T>
  void set(const std::string& name, T val) {
    std::map<std::string, Sym>::iterator i = f.find(name);
    if (i == f.end()) {
      f[name] = Sym(val);
      return;
    }
    // const char *i->first = name.c_str();
    //*i->first = name.c_str();
    f[name] = Sym(val);
    // i->second = Sym(val);
  }

  uint16_t getUInt16(const std::string& name) const {
    return getSym(name, __LINE__)->u16;
  }

  uint32_t getUInt32(const std::string& name) const {
    return getSym(name, __LINE__)->u32;
  }

  uint64_t getUInt64(const std::string& name) const {
    return getSym(name, __LINE__)->u64;
  }

  int16_t getInt16(const std::string& name) const {
    return getSym(name, __LINE__)->i16;
  }

  int32_t getInt32(const std::string& name) const {
    return getSym(name, __LINE__)->i32;
  }

  int64_t getInt64(const std::string& name) const {
    return getSym(name, __LINE__)->i64;
  }

  bool getBool(const std::string& name) const {
    return getSym(name, __LINE__)->b;
  }

  double getDouble(const std::string& name) const {
    return getSym(name, __LINE__)->d;
  }

  std::string getString(const std::string& name) const {
    return *getSym(name, __LINE__)->s;
  }
  void mandatory(int count...);
  void optional(int count...);
};

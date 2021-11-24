#include <array>
#include <iostream>
#include <string>
#include <cstring>
#include <memory>
using namespace std;

enum class Type {
  U8,
  U16,
  U32,
  U64,
  I8,
  I16,
  I32,
  I64,
  F32,
  F64,
  STRING8,
  STRUCT,
  LIST,
  SIZE_OF_TYPES
};

const char* typenames[] = {
  "U8",
  "U16",
  "U32",
  "U64",
  "I8",
  "I16",
  "I32",
  "I64",
  "F32",
  "F64",
  "STRING8",
  "STRUCT",
  "LIST"
};

class XDLType {
private:
    string name;
  public:
    XDLType(const string& name) : name(name) {}
    string getName() const { return name; }
};

class U32 : public XDLType {
  private:
    uint32_t v;
  public:
    U32(uint32_t v) : XDLType("U32"), v(v) {}
};

uint32_t operator"" _32(const char* s) {
  cout << s << '\n';
  return atoi(s);
}

uint64_t operator"" _u64(const char* s) {
  cout << s << '\n';
  return atol(s);
}

float operator"" _f32(const char* s) {
  cout << s << '\n';
  return atof(s);
}

double operator"" _f64(const char* s) {
  cout << s << '\n';
  return atof(s);
}

void write(char*& buf, uint8_t val) {
  *(uint8_t*)buf = val;
  buf += sizeof(uint8_t);
}

void write(char*& buf, uint16_t val) {
  *(uint16_t*)buf = val;
  buf += sizeof(uint16_t);
}

void write(char*& buf, uint32_t val) {
  *(uint32_t*)buf = val;
  buf += sizeof(uint32_t);
}

void write(char*& buf, uint64_t val) {
  *(uint64_t*)buf = val;
  buf += sizeof(uint64_t);
}

void write(char*& buf, float val) {
  *(float*)buf = val;
  buf += sizeof(float);
}
void write(char*& buf, double val) {
  *(double*)buf = val;
  buf += sizeof(double);
}

void writeMeta(char*& buf, uint8_t val) {
  *(uint8_t*)buf = uint32_t(Type::U8);
  buf += sizeof(uint8_t);
}

void writeMeta(char*& buf, uint16_t val) {
  *(uint8_t*)buf = uint32_t(Type::U16);
  buf += sizeof(uint8_t);
}

void writeMeta(char*& buf, uint32_t val) {
  *(uint8_t*)buf = uint32_t(Type::U32);
  buf += sizeof(uint8_t);
}

void writeMeta(char*& buf, uint64_t val) {
  *(uint8_t*)buf = uint32_t(Type::U64);
  buf += sizeof(uint8_t);
}

void writeMeta(char*& buf, float val) {
  *(uint8_t*)buf = uint32_t(Type::F32);
  buf += sizeof(uint8_t);
}
void writeMeta(char*& buf, double val) {
  *(uint8_t*)buf = uint32_t(Type::F64);
  buf += sizeof(uint8_t);
}

class Point {
 public:
  double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) :  x(x), y(y), z(z) {}
};

void write(char*& buf, const char name[]) {
  uint32_t len = strlen(name);
  *buf++ = len; // strings must be <256 chars for this demo
  memcpy(buf, name, len);
  buf += len;
}

void write(char*& buf, const string& name) {
  uint32_t len = name.length();
  *buf++ = len;// strings must be <256 chars for this demo
  memcpy(buf, name.c_str(), len);
  buf += len;
}

void write(char*& buf, const Point& p) {
  double* d = (double*)buf;
  *d++ = p.x;
  *d++ = p.y;
  *d++ = p.z;
  buf = (char*)d;
}
void writeMeta(char*& buf, Type t, const char name[]) {
  *buf++ = (char)t;
  write(buf, name);
}

void writeMeta(char*& buf, const Point& p) {
  *buf++ = (char)Type::STRUCT;
  write(buf, "Point");
  *buf++ = 3; // 3 fields
  writeMeta(buf, Type::F64, "x");
  writeMeta(buf, Type::F64, "y");
  writeMeta(buf, Type::F64, "z");
}

class StockQuote {
  public:
    uint32_t date;
    float open, close, high, low;
    StockQuote(uint32_t date, float open, float close, float high, float low) : date(date), open(open), high(high), low(low) {}
};

void write(char*& buf, const StockQuote& q) {


}

void writeMeta(char*& buf, const StockQuote& q) {
  *buf++ = (char)Type::STRUCT;
  write(buf, "StockQuote");
  *buf++ = 5;
  writeMeta(buf, Type::U32, "date"); // should be Date!
  writeMeta(buf, Type::F32, "open");
  writeMeta(buf, Type::F32, "close");
  writeMeta(buf, Type::F32, "high");
  writeMeta(buf, Type::F32, "low");
}

// C++17 ugh!
template <typename... Args>
void writeBuf(char*& buf, char*& meta, const Args&... args) {
  (write(buf, args), ...);
  (writeMeta(meta, args), ...);
}

template<typename T>
void writeBoth(char*& buf, char*& meta, const T& v) {
  write(buf, v);
  writeMeta(meta, v);
}

template<typename T>
class List : public XDLType {
  private:
    uint32_t capacity;
    uint32_t size_;
    T* data; // quick hack for this MWE, generic list contains block of objects of type T, not optimal
    void checkGrow() {
      if (size_ < capacity) {
        return;
      }
      const T* old = data;
      data = new T[capacity*=2];
      memcpy(data, old, size_ * sizeof(T));
    }
  public:
    List(const string& name, uint32_t capacity = 16) : XDLType(name), capacity(capacity), size_(0), data(new T[capacity]){}
    ~List() { delete [] data; }
    List(const List& orig) = delete; // don't support copying for this MWE
    List& operator=(const List& orig) = delete; // don't support copying for this MWE

    void add(const T& e) {
      checkGrow();
      data[size_++] = e;
    }
    const T& operator [](uint32_t i) const {
      return data[i];
    }
    uint32_t size() const { return size_; }
};

template<typename T>
void write(char*& buf, const List<T>& list) {
  write(buf, uint8_t(list.size())); // only handle lists up to 255 elements
  for (uint32_t i = 0; i < list.size(); i++)
    write(buf, list[i]);
}

template<typename T>
void writeMeta(char*& buf, const List<T>& list) {
  *buf++ = (char)Type::LIST;
  write(buf, list.getName());
  writeMeta(buf, list[0]);
}

#if 0
// C++17
template <typename... Ts>
void test(Ts... args) {
  ((std::cout << sizeof(first) << '\n'), ...);
}
#endif

#if 0
// C++11



void writeBuf(char* buf, char* meta) {}

template <typename T, typename... Args>
void writeBuf(char* buf, char* meta, T first, Args... args) {
  write(buf, first);
  writeMeta(meta, first);
  writeBuf(buf, meta, args...);
}
#endif

#if 0
template <typename... Ts>
constexpr std::array<Type, sizeof...(Ts)> getMeta() {
  return {{toType<Ts>...}};
}
#endif

void dump(const char msg[], const char buf[], uint32_t len) {
  cout << msg;
  for (int i = 0; i < len; i++) {
    cout << hex << uint32_t(uint8_t(buf[i])) << ' ';
  }
  cout << '\n';
}

void dumpStr() {
}

// nicer dump of metadata, aware of what the types mean?
void dumpMeta(const char buf[], uint32_t len) {
  cout << "Metadata: ";
  uint32_t i = 0;
  while (i < len) {
    if (buf[i] < uint32_t(Type::SIZE_OF_TYPES))
      cout << typenames[buf[i]] << ' ';
    else {
      cerr << "Found illegal typecode:" << uint32_t(buf[i]) << ' ';
      continue;
    }
    switch(Type(buf[i])) {
      case Type::STRUCT:
 //       dumpString(buf);
        break;
      case Type::LIST:
        break;
      case Type::STRING8:
        break;
      default:
        break;
    }
  }
}

int main() {
  char buffer[1024];
  char metadata[1024];
  Point p(1, 2, 3);
  List<uint32_t> numbers("numpy");
  numbers.add(3);
  numbers.add(1);
  numbers.add(4);

  List<Point> points("fred");
  for (int i = 0; i < 2; i++)
    points.add(Point(i,i+1,i+2));

  char* buf = buffer;
  char* meta = metadata;

  writeBuf(buf, meta,
           uint32_t(2),
           uint64_t(123456789012345678ULL),
           1.5,
           p,
           points,
           numbers);

  dump("data: ", buffer, buf-buffer);
  dump("metadata: ", metadata, meta - metadata);
}

#include <array>
#include <iostream>
#include <string>
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
  STRUCT,
  LIST,
  STRING8
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
  Point(double x, double y, double z) : x(x), y(y), z(z) {}
};

void write(char*& buf, const Point& p) {
  *(double*)buf++ = p.x;
  *(double*)buf++ = p.y;
  *(double*)buf++ = p.z;
}
void writeMeta(char*& buf, const Point& p) {
  constexpr uint32_t meta =
      uint32_t(Type::STRUCT) | (uint32_t(Type::F64) << 8) |
      (uint32_t(Type::F64) << 8) | (uint32_t(Type::F64) << 8);
  *(uint32_t*)buf = meta;
  //	strcpy(buf, p.
  buf += sizeof(uint32_t);
}

// C++17 ugh!
template <typename... Args>
void writeBuf(char* buf, char* meta, Args... args) {
  write(buf, args);
  writeMeta(meta, args);
  args...;
}

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

int main() {
  char buffer[1024];
  char metadata[1024];
  Point p(1, 2, 3);
  writeBuf(buffer, metadata, uint32_t(2), uint64_t(3), p, 3.5, 1.5f);
}
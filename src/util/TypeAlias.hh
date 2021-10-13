#include <cstdint>
#include <limits>
#include <string>

#include "fmt/core.h"

using namespace std;

uint16_t operator"" _u16(const char* s) { return stoul(s); }

constexpr uint16_t operator"" _u16(unsigned long long t) {
  if (t > numeric_limits<uint16_t>::max()) {
    fmt::print(stderr, "{} is out of bounds for an unsigned 16-bit integer.\n",
               t);
  }
  return t;
}

int16_t operator"" _i16(const char* s) { return stoi(s); }

constexpr int16_t operator"" _i16(unsigned long long t) {
  if (t > numeric_limits<int16_t>::max() ||
      t < numeric_limits<int16_t>::min()) {
    fmt::print(stderr, "{} is out of bounds for a signed 16-bit integer.\n", t);
  }
  return t;
}

uint32_t operator"" _u32(const char* s) { return stoul(s); }

constexpr uint32_t operator"" _u32(unsigned long long t) {
  if (t > numeric_limits<uint32_t>::max()) {
    fmt::print(stderr, "{} is out of bounds for an unsigned 32-bit integer.\n",
               t);
  }
  return t;
}

int32_t operator"" _i32(const char* s) { return stol(s); }

constexpr int32_t operator"" _i32(unsigned long long t) {
  if (t > numeric_limits<int32_t>::max() ||
      t < numeric_limits<int32_t>::min()) {
    fmt::print(stderr, "{} is out of bounds for a signed 32-bit integer.\n", t);
  }
  return t;
}

uint64_t operator"" _u64(const char* s) { return stoull(s); }

constexpr uint64_t operator"" _u64(unsigned long long t) {
  if (t > numeric_limits<uint64_t>::max()) {
    fmt::print(stderr, "{} is out of bounds for an unsigned 64-bit integer.\n",
               t);
  }
  return t;
}

int64_t operator"" _i64(const char* s) { return stoll(s); }

constexpr int64_t operator"" _i64(unsigned long long t) {
  if (t > numeric_limits<int64_t>::max() ||
      t < numeric_limits<int64_t>::min()) {
    fmt::print(stderr, "{} is out of bounds for a signed 64-bit integer.\n", t);
  }
  return t;
}

float operator"" _f32(const char* s) { return atof(s); }

constexpr float operator"" _f32(long double t) {
  if (t > numeric_limits<float>::max() || t < numeric_limits<float>::min()) {
    fmt::print(stderr, "{} is out of bounds for a 32-bit float.\n", t);
  }

  return t;
}

double operator"" _f64(const char* s) { return atof(s); }

constexpr double operator"" _f64(long double t) {
  if (t > numeric_limits<double>::max() || t < numeric_limits<double>::min()) {
    fmt::print(stderr, "{} is  out of bounds for a 64-bit float.\n", t);
  }

  return t;
}

double operator"" _f128(const char* s) { return atof(s); }

constexpr long double operator"" _f128(long double t) {
  if (t > numeric_limits<long double>::max() ||
      t < numeric_limits<long double>::min()) {
    fmt::print(stderr,
               "{} is  out of bounds for a long double-precision float (may be "
               "128 bits or 64 bits).\n",
               t);
  }

  return t;
}
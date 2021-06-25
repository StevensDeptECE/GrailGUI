#include "TypeAlias.hh"

#include <iostream>

using namespace std;

constexpr uint32_t operator"" _32(const char* s) {
  cout << s << '\n';
  return atoi(s);
}

constexpr uint32_t operator"" _32(uint64_t t) { return t; }

constexpr uint64_t operator"" _su64(const char* s) {
  cout << s << '\n';
  return atol(s);
}

constexpr uint64_t operator"" _u64(uint64_t t) { return t; }

constexpr float operator"" _f32(const char* s) {
  cout << s << '\n';
  return atof(s);
}

constexpr float operator"" _f32(long double t) { return t; }

constexpr double operator"" _f64(const char* s) {
  cout << s << '\n';
  return atof(s);
}

constexpr double operator"" _f64(long double t) { return t; }
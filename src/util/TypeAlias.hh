#include <cstdint>

constexpr uint32_t operator"" _u32(const char* s);
constexpr uint32_t operator"" _u32(uint64_t t);

constexpr uint64_t operator"" _u64(const char* s);
constexpr uint64_t operator"" _u64(uint64_t t);

constexpr float operator"" _f32(const char* s);
constexpr float operator"" _f32(long double t);

constexpr double operator"" _f64(const char* s);
constexpr double operator"" _f64(long double t);
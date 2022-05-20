#pragma once
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <time.h>

#include <chrono>
#include <cstdint>
#include <functional>
#include <iostream>

#include "util/Ex.hh"

namespace grail {
namespace utils {
namespace {
void dropCaches() {
  int status = system("echo 3 | sudo tee /proc/sys/vm/drop_caches > /dev/null");
}

std::string trim_nonalpha(const std::string& s) {
  std::string result;
  result.reserve(s.length());

  std::remove_copy_if(s.begin(), s.end(), std::back_inserter(result),
                      std::not_fn(std::function(isalpha)));

  return result;
}
}  // namespace

template <typename Ratio = std::milli>
class CBenchmark {
 private:
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
      t0;
  std::chrono::duration<double, Ratio> elapsedTime;
  const std::string_view msg;

 public:
  CBenchmark(const std::string_view msg) : msg(msg), elapsedTime(0) {}

  constexpr void reset() { elapsedTime.zero(); }

  void start() { t0 = std::chrono::steady_clock::now(); }

  void end() {
    auto t1 = std::chrono::steady_clock::now();
    elapsedTime += (t1 - t0);
  }

  auto elapsed() const { return elapsedTime; }
  void display() const {
    fmt::print("{} elapsed time: {}\n", msg, elapsedTime);
  }
  void displayavg(uint64_t iterations) const {
    fmt::print("{} elapsed time: {}\n", msg, elapsedTime / iterations);
  }

  template <typename Func>
  static void benchmark(const std::string& msg, uint64_t numIterations,
                        Func func) {
    uint64_t iter = numIterations;
    CBenchmark b(msg);
    b.start();
    for (; numIterations > 0; numIterations--) func();
    b.end();
    b.displayavg(iter);
  }

  template <typename Func>
  static void benchmarkNoCache(const std::string_view msg,
                               const uint64_t numIterations, const Func func,
                               const bool mute_warnings = false) {
    uint64_t iter = numIterations;
    if (!mute_warnings) {
      fmt::print("Warning: dropping caches requires root access,");
      fmt::print(" be prepared for a sudo prompt.\n");
      fmt::print("Are you sure that you want to proceed (Y/n)? ");
      std::string consent;
      std::cin >> consent;
      consent = trim_nonalpha(consent);
      if (consent.length() != 1 || tolower(consent[0]) != 'y') {
        throw Ex1(Errcode::PERMISSION_DENIED);
      }
    }
    CBenchmark<Ratio> b(msg);
    for (; iter > 0; iter--) {
      b.start();
      func();
      b.end();
      dropCaches();
    }
    b.displayavg(numIterations);
  }
};
};  // namespace benchmark
};  // namespace grail

#pragma once
#include <fmt/core.h>
#include <sys/times.h>
#include <time.h>

#include <chrono>
#include <cstdint>
#include <iostream>

class Benchmark {
 private:
  uint64_t elapsedUserTime, elapsedSystemTime, elapsedWallTime,
      elapsedClockTicks;
  int BENCHCLOCKS_PER_SEC;
  struct tms startCPUTimes;
  time_t startWallTime;
  struct tms endCPUTimes;
  time_t endWallTime;
  clock_t t;

 public:
  // measure how long it takes to execute a function
  void display() const {
    std::cout << "User:        "
              << elapsedUserTime / double(BENCHCLOCKS_PER_SEC) * 1E9 << '\n'
              << "System:      "
              << elapsedSystemTime / double(BENCHCLOCKS_PER_SEC) * 1E9 << '\n'
              << "Wall:        " << elapsedWallTime << '\n'
              << "Clock Ticks: " << elapsedClockTicks << '\n';
  }

  // display avg in nanoseconds
  void displayavg(uint64_t iterations) const {
    std::cout << "User:        "
              << (elapsedUserTime / double(BENCHCLOCKS_PER_SEC)) / iterations *
                     1E9
              << '\n'
              << "System:      "
              << (elapsedSystemTime / double(BENCHCLOCKS_PER_SEC)) /
                     iterations * 1E9
              << '\n'
              << "Wall:        " << elapsedWallTime << '\n'
              << "Clock Ticks: " << elapsedClockTicks << '\n';
  }

  friend std::ostream& operator<<(std::ostream& s, const Benchmark& b) {
    return s << "User:        "
             << b.elapsedUserTime / double(b.BENCHCLOCKS_PER_SEC) << '\n'
             << "System:      "
             << b.elapsedSystemTime / double(b.BENCHCLOCKS_PER_SEC) << '\n'
             << "Wall:        " << b.elapsedWallTime << '\n'
             << "Clock Ticks: " << b.elapsedClockTicks << '\n';
  }

  Benchmark()
      : elapsedUserTime(0),
        elapsedSystemTime(0),
        elapsedWallTime(0),
        elapsedClockTicks(0),
        BENCHCLOCKS_PER_SEC(sysconf(_SC_CLK_TCK)) {}

  void start() {
    times(&startCPUTimes);
    time(&startWallTime);

    t = clock();
  }

  void end() {
    times(&endCPUTimes);
    time(&endWallTime);
    elapsedUserTime += endCPUTimes.tms_utime - startCPUTimes.tms_utime;
    elapsedSystemTime += endCPUTimes.tms_stime - startCPUTimes.tms_stime;
    elapsedWallTime += endWallTime - startWallTime;

    elapsedClockTicks = clock() - t;
  }

  template <typename Func>
  static void benchmark(Func func) {
    Benchmark a;
    a.start();
    func();
    a.end();
    std::cout << a << '\n';
  }

  template <typename Func>
  constexpr static void benchmark(const std::string& msg,
                                  uint64_t numIterations, Func func) {
    Benchmark a;
    uint64_t iter = numIterations;
    a.start();
    for (; numIterations > 0; numIterations--) func();
    a.end();
    std::cout << msg << '\n';
    a.displayavg(iter);
  }
};

class CBenchmark {
 private:
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
      t0;
  double elapsedTime;
  std::string msg;

 public:
  CBenchmark(const std::string& msg) : msg(msg) { reset(); }

  void reset() { elapsedTime = 0; }

  void start() {
    t0 = time_point_cast<std::chrono::nanoseconds>(
        std::chrono::steady_clock::now());
  }

  void end() {
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>
        t1 = time_point_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now());
    elapsedTime += (t1 - t0).count();
  }

  constexpr double elapsed() { return elapsedTime; }
  void display() const {
    fmt::print("{} elapsed time: {}\n", msg, elapsedTime);
  }
  void displayavg(uint64_t iterations) const {
    fmt::print("{} elapsed time: {}\n", msg, elapsedTime / iterations);
  }

  template <typename Func>
  constexpr static void benchmark(const std::string& msg,
                                  uint64_t numIterations, Func func) {
    uint64_t iter = numIterations;
    CBenchmark b(msg);
    b.start();
    for (; numIterations > 0; numIterations--) func();
    b.end();
    b.displayavg(iter);
  }
};
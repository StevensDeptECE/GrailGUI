#include <chrono>

// TODO: Consider deleting this, Benchmark.hh does a very similar thing

class CTimer {
 public:
  CTimer() : beg_(clock_::now()) {}
  void reset() { beg_ = clock_::now(); }
  double elapsedSeconds() const;
  double elapsedMillis() const;
  double elapsedMicros() const;
  double elapsedNanos() const;

 private:
  typedef std::chrono::steady_clock clock_;
  typedef std::chrono::duration<double> s_;
  typedef std::chrono::duration<double, std::milli> ms_;
  typedef std::chrono::duration<double, std::micro> us_;
  typedef std::chrono::duration<double, std::nano> ns_;
  std::chrono::time_point<clock_> beg_;
};
#include "util/Timers.hh"

double CTimer::elapsedSeconds() const {
  return duration_cast<s_>(clock_::now() - beg_).count();
}
double CTimer::elapsedMillis() const {
  return duration_cast<ms_>(clock_::now() - beg_).count();
}
double CTimer::elapsedMicros() const {
  return duration_cast<us_>(clock_::now() - beg_).count();
}
double CTimer::elapsedNanos() const {
  return duration_cast<ns_>(clock_::now() - beg_).count();
}
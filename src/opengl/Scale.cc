#include "Scale.hh"

#include <cmath>
#include <iostream>

float LinearScale::transform(double x) { return scale * x + shift; }

float LogScale::transform(double x) { return scale * log10(x) + shift; }

void LinearScale::init(double min, double max, double shift, double axisSize,
                       double tickInterval) {
  this->min = min;
  this->max = max;
  this->tickInterval = tickInterval;
  this->numInterval = tickInterval;
  scale = axisSize / (max - min);
  this->shift = scale * (-min) + shift;
}

void LogScale::init(double min, double max, double shift, double axisSize,
                    double tickInterval) {
  this->min = min;
  this->max = max;
  this->tickInterval = tickInterval;
  this->numInterval = tickInterval;
  if (min == 0) {
    min = 1;
  }
  scale = axisSize / (log10(max) - log10(min));
  this->shift = shift;
}

float LinearScale::next(float v) { return v + tickInterval; }

float LogScale::next(float v) {
  if (v == 0) {
    return 1;
  }
  return v * tickInterval;
}

// test whether things scale right
#if 0
int main() {
  float x[] = {1.0, 10.0, 100.0, 1000.0};
  float y[] = {0, 0, 0, 0};
  LogarithmicScale thing(1, 0);
  for (int i = 0; i < 4; i++) {
    y[i] = thing.transform(x[i]);
  }

  for (int i = 0; i < 4; i++) {
    std::cout << y[i] << '\n';
  }
}
#endif

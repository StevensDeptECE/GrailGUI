#pragma once
#include "util/DynArray.hh"
// TODO: Write a truly generic arbitrary function instead of cubic?
class CubicFunc {
 private:
  DynArray<float> coef;  // coefficients for bezier and spline segments
 public:
  double arcLength() const;           // return length of this function
  double segmentLength(int i) const;  // return length of 1 segment
  double deriv(int i, double t) const {
    const int seg = i * 8;
    float a = coef[seg], b = coef[seg + 1], c = coef[seg + 2],
          d = coef[seg + 3];
    double x = (((a * t + b) * t + c)* t + d;
  }
};

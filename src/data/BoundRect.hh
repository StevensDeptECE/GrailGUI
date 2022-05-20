#pragma once
#include <iostream>
#include <utility>

class BoundRect {
 public:
  float xMin, xMax, yMin, yMax;
  // TODO: not great style, we allow this to be random and fill in later, but
  // anything else would be very wasteful. we could allow special friend classes
  // like BlockMapLoader to do this.
  BoundRect() {}
  BoundRect(float xMin, float xMax, float yMin, float yMax)
      : xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax) {}

  static BoundRect merge(const BoundRect& a, const BoundRect& b) {
    return BoundRect(std::min(a.xMin, b.xMin), std::max(a.xMax, b.xMax),
                     std::min(a.yMin, b.yMin), std::max(a.yMax, b.yMax));
  }

  // returns true if a is completely contained within this BoundRect
  bool contains(const BoundRect& a) {
    return a.xMin >= xMin && a.xMin <= xMax && a.yMin >= yMin &&
           a.yMin <= yMax && a.xMax >= xMin && a.xMax <= xMax &&
           a.yMax >= yMin && a.yMax <= yMax;
  }

  // returns true if any part of a is within this BoundRect
  bool intersects(const BoundRect& a) {
    return (a.xMin >= xMin && a.xMin <= xMax ||
            a.xMax >= xMin && a.xMax <= xMax) &&
           (a.yMin >= yMin && a.yMin <= yMax ||
            a.yMax >= yMin && a.yMax <= yMax);
  }
  friend std::ostream& operator<<(std::ostream& s, const BoundRect& b) {
    return s << '[' << b.xMin << ',' << b.xMax << " | " << b.yMin << ','
             << b.yMax << ']';
  }
};

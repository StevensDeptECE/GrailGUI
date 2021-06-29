#pragma once

class BoundRect {
 public:
  float xMin, xMax, yMin, yMax;
  BoundRect(float xMin, float xMax, float yMin, float yMax)
      : xMin(xMin), xMax(xMax), yMin(yMin), yMax(yMax) {}
};
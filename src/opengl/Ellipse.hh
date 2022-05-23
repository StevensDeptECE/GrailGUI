#pragma once

#include "opengl/Sector.hh"

class Ellipse : public Sector {
 private:
 public:
  Ellipse(float x, float y, float xRad, float yRad, float angleInc, Style* s)
      : Sector(x, y, xRad, yRad, 0, 361, angleInc, s) {}
};

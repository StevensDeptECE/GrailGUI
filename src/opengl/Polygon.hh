#pragma once

#include "opengl/Sector.hh"

class Polygon : public Sector {
 private:
 public:
  Polygon(float x, float y, float xRad, float yRad, float n, Style* s)
      : Sector(x, y, xRad, yRad, 0, 360, 360 / n, s) {}
};

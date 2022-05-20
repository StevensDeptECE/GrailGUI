#pragma once

#include "opengl/Shape2D.hh"

class Line : public Shape2D {
 private:
 public:
  Line(Canvas* c, float x1, float y1, float x2, float y2, Style* s)
      : Shape2D(c, x1, y1, s) {
    addPoint(x1, y1);
    addPoint(x2, y2);

    initIndices();
  }
  void initIndices();
  void render();
};

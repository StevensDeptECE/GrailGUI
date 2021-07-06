#pragma once
#include "opengl/GrailGUI.hh"

class StyledMultiShape2D;
class MultiText;

class SuperWidget2D {
 protected:
  Canvas* c;
  float x, y;  // top left corner of the widget on the screen
  float w, h;  // width and height of the widget
 public:
  SuperWidget2D(Canvas* c, float x, float y, float w, float h)
      : c(c), x(x), y(y), w(w), h(h) {}
  virtual void init() = 0;
};

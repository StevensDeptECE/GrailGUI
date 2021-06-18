#pragma once
#include "opengl/GrailGUI.hh"

/*
  Represent a compound object to be drawn using StyledMultiShape2D and MultiText

*/

class StyledMultiShape2D;
class MultiText;

class SuperWidget2D {
 protected:
  Canvas* c;
  StyledMultiShape2D* m;
  MultiText* t;
  float x, y;  // top left corner of the widget on the screen
  float w, h;  // width and height of the widget
 public:
  SuperWidget2D(Canvas* c, StyledMultiShape2D* m, MultiText* t, float x,
                float y, float w, float h)
      : c(c), m(m), t(t), x(x), y(y), w(w), h(h) {}
  virtual void init() = 0;
};

#pragma once
#include "opengl/GrailGUI.hh"

/*
  Represent a compound object to be drawn using StyledMultiShape2D and MultiText

*/

class StyledMultiShape2D;
class MultiText;

class Widget2D {
protected:
  StyledMultiShape2D* m;
  MultiText* t;
  float x,y; // top left corner of the widget on the screen
  float w,h; // width and height of the widget
public:
  Widget2D(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) : m(m), t(t), x(x), y(y), w(w), h(h) {}
  virtual void init() = 0;
};

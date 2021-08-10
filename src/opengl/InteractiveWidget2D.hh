#pragma once
#include "fmt/core.h"
#include "opengl/Widget2D.hh"

/*
  Represent a compound object to be drawn using StyledMultiShape2D and MultiText

*/

class InteractiveWidget2D : public Widget2D {
 protected:
  MainCanvas* c;
  bool isPressed;

 public:
  InteractiveWidget2D(MainCanvas* c, float x, float y, float w, float h)
      : Widget2D(c->getGui(), c->getGuiText(), x, y, w, h),
        c(c),
        isPressed(false) {
    c->addClickableWidget(this);
  }

  virtual void init() = 0;

  virtual void click(float mouseX, float mouseY) = 0;
  bool checkClick(float mouseX, float mouseY) {
    fmt::print("({},{})\n", mouseX, mouseY);
    if (mouseX < x || mouseY < y || mouseX > x + w || mouseY > y + h) {
      isPressed = false;
      return false;
    }
    click(mouseX, mouseY);
    return true;
  }
};

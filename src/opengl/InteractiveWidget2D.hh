#pragma once
#include "opengl/Widget2D.hh"

/*
  Represent a compound object to be drawn using StyledMultiShape2D and MultiText

*/

class InteractiveWidget2D : public Widget2D {
 protected:
 public:
  InteractiveWidget2D(MainCanvas* c, float x, float y, float w, float h)
      : Widget2D(c->getGui(), c->getGuiText(), x, y, w, h) {
    c->addClickableWidget(this);
  }
  virtual void click(float xPress, float yPress, float xRelease,
                     float yRelease) = 0;
  bool checkClick(float xPress, float yPress, float xRelease, float yRelease) {
    if (xPress < x || xRelease < x || yPress < y || yRelease < y ||
        xPress > x + w || xRelease > x + w || yPress > y + h ||
        yRelease > y + h)
      return false;
    click(xPress, yPress, xRelease, yRelease);
    return true;
  }
};

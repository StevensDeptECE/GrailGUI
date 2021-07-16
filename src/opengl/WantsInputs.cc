#include "opengl/WantsInputs.hh"

#include "opengl/Canvas.hh"

WantsInputs::WantsInputs(Canvas* c) {
  mycanvas = c;
  mycanvas->getWin()->addWantsInputs(this);
}

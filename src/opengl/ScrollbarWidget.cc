#include "opengl/ScrollbarWidget.hh"

#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"

using namespace std;

void ScrollbarWidget::init() {
  // TODO: draw the ScrollbarWidget
  m->drawRectangle(x, y, w, h, grail::gray);
  m->fillRectangle(x, y, w, h, grail::white);
  m->drawRectangle(x, y, w, h / 2, grail::gray);
  m->fillRectangle(x, y, w, h / 2, grail::gray);
}
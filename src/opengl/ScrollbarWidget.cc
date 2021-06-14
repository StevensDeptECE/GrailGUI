#include "opengl/ScrollbarWidget.hh"

#include "opengl/GLWin.hh"
#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"

using namespace std;

void ScrollbarWidget::init() {
  // TODO: draw the ScrollbarWidget

  drawRectangle(0, 0, 500, 500, grail::red);
  drawRectangle(x, y, w, h, grail::gray);
  fillRectangle(x, y, w, h, grail::blue);
  drawRectangle(x, y, w, h / 2, grail::red);
  fillRectangle(x, y, w, h / 2, grail::gray);
  // update();
}

void ScrollbarWidget::render() { StyledMultiShape2D::render(); }
void ScrollbarWidget::update() {}
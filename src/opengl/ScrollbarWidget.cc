#include "opengl/ScrollbarWidget.hh"

#include "opengl/GLWin.hh"
#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"

using namespace std;

void ScrollbarWidget::scroll(float dy) {
  scrollbarBoxY += dy;
  if (scrollbarBoxY < y) {
    scrollbarBoxY = y;
  }
  if (scrollbarBoxY > y + h - boxSize) {
    scrollbarBoxY = y + h - boxSize;
  }
}

void ScrollbarWidget::draw() {
  drawRectangle(x, y, w, h, grail::gray);
  fillRectangle(x, y, w, h, grail::blue);
  drawRectangle(x, scrollbarBoxY, w, boxSize, grail::red);
  fillRectangle(x, scrollbarBoxY, w, boxSize, grail::gray);
}

void ScrollbarWidget::init() {
  // TODO: draw the ScrollbarWidget
  draw();
  StyledMultiShape2D::init();
  // update();
}

void ScrollbarWidget::render(glm::mat4& trans) { StyledMultiShape2D::render(trans); }
void ScrollbarWidget::update() {
  clear();
  draw();
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0],
               GL_DYNAMIC_DRAW);
}
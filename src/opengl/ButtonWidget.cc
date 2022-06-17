#include "opengl/ButtonWidget.hh"

#include <iostream>

#include "fmt/core.h"
#include "opengl/GLWin.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/Tab.hh"
using namespace std;

void ButtonWidget::click(float mouseX, float mouseY) {
  isPressed = !isPressed;
  if (isPressed) {
    fmt::print("Button clicked {} at ({}, {})\n", text, mouseX, mouseY);
    if (func) {
      // 1
      auto funcptr = func.value();
      funcptr();
    }
  }
}

void ButtonWidget::init() {}

ButtonWidget::ButtonWidget(MainCanvas* c, const Style* s, float x, float y,
                           float w, float h, const std::string& text,
                           const char action[])
    : InteractiveWidget2D(c, s, x, y, w, h), text(text) {
  redraw();
}

ButtonWidget::ButtonWidget(MainCanvas* c, const Style* s, float x, float y,
                           const std::string& text, const char action[])
    : InteractiveWidget2D(c, s, x, y, s->f->getWidth(text), s->f->getHeight()),
      text(text) {
  redraw();
}

void ButtonWidget::redraw() {
  float borderSize = s->lineWidth;

  m->fillRectangle(x - borderSize, y - borderSize, w + (borderSize * 2),
                   h + (borderSize * 2), s->bg);
  m->fillRectangle(x, y, w, h, s->fg);
  t->addCentered(x, y, w, h, s->f, text);
}
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
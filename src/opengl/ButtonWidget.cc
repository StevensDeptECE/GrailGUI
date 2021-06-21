#include "opengl/ButtonWidget.hh"

#include <iostream>

#include "opengl/GLWin.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/Tab.hh"
using namespace std;

void ButtonWidget::init() {}

void ButtonWidget::click(float xPress, float yPress, float xRelease,
                         float yRelease) {
  cout << "Button clicked" << text;
}
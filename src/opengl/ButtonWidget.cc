#include "opengl/GrailGUI.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include "opengl/GLWin.hh"
#include "opengl/Tab.hh"

using namespace std;

void ButtonWidget::init() {
  int x = 300;
  int y = 300;
  int w = 200;
  int h = 100;
  int borderSize = 2;

  m->fillRectangle(x-borderSize, y-borderSize, w+(borderSize*2), h+(borderSize*2), grail::black);
  m->fillRectangle(x, y, w, h, grail::red);
  t->add(x+(w/2),y+(h/2),"aaa",3);



}
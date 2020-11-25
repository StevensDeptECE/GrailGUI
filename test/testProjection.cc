#include "grail/Grail.hh"
#include "opengl/Cursor.hh"
#include <iostream>

using namespace std;

void init(shared_ptr<CompiledWeb> cw) {
  uint32_t sizes[] = {12,14,16,30,40};
  FontFace::loadFonts("TIMES",sizes,5);
  cw->getParent()->inp.init();
  cw->getParent()->inp.assign_mouse("Click for mouse 1\n", 0);
  cw->getParent()->inp.start();

  const Font* font = FontFace::get("TIMES", 40, FontFace::BOLD);

  Style* cursorStyle = new Style(font, 1,0,0, 0,0,0);
  cursorStyle->setLineWidth(1);


  Style* style = new Style(font, 0,0,1, 0,0,0);
  style->setLineWidth(1);
  
  Style* style1 = new Style(font, 0,0,1, 0,0,0);
  style1->setLineWidth(1);
  style1->setShaderIndex(CompiledWebWindow::COMMON_SHADER);

  // Canvas* c = cw_ptr->getMainCanvas();
  Canvas *c = cw->addCanvas(style,0,0,500,500,GLWin::Width,GLWin::Height);
  Canvas *c1 = cw->addCanvas(style1,0,0,500,500,500,500);
  MultiShape2D *m = c->addLayer(new MultiShape2D(style));
  MultiShape2D *m1 = c1->addLayer(new MultiShape2D(style1));
  m->drawLine(100, 100, 500, 100);
  m->drawLine(100, 200, GLWin::Width, 200);
  m1->drawLine(100, 300, 500, 300);
  m1->drawLine(100, 400, GLWin::Width, 400);

  
  Canvas* cursorCanvas = cw->addCanvas(cursorStyle,0,0,GLWin::Width,GLWin::Height,GLWin::Width,GLWin::Height);
  cursorCanvas->addLayer(new Cursor(20,20,cursorStyle,0));
}

int main(int argc, char *argv[]) {
  int ret = CompiledWebWindow::initGrail(argc, argv, init);
  return ret;
}

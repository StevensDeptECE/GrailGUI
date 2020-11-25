#define STB_IMAGE_IMPLEMENTATION
#include "grail/Grail.hh"
#include <stdio.h>
using namespace std;

void make_mouse(shared_ptr<CompiledWeb> cw);

void init(shared_ptr<CompiledWeb> cw) {
  const uint32_t w = GLWin::Width, h = GLWin::Height;

  make_mouse(cw);

 // uint32_t sizes[] = {12,14,16,30,40};
  //FontFace::loadFonts("TIMES",sizes,5);
  const Font* font = FontFace::get("TIMES", 16, FontFace::BOLD);

  Style* tableShader = new Style(font, 0,0,0, 0,0,0);
  tableShader->setLineWidth(1);
  tableShader->setShaderIndex(CompiledWebWindow::TEXTURE_SHADER);

  Style *textShader = new Style(font, 0,0,0, 0,0,0);
  textShader->setLineWidth(1);
  textShader->setShaderIndex(CompiledWebWindow::TEXT_SHADER);

  /* Table */
  const uint32_t offset_x = 100;
  const uint32_t offset_y = 100;
  const uint32_t tHeight = 50;
  const uint32_t tWidth = 150;
  const uint32_t numRows = 10;
  const uint32_t numCols = 05;
  const char *text[][numCols] = {
    { "Time", "Velocity", "Position", "Acceleration", "Jerk" },
    { "0", "1",  "2",  "3",  "4" },
    { "1", "2",  "3",  "4",  "5" },
    { "2", "3",  "4",  "5",  "6" },
    { "3", "4",  "5",  "6",  "7" },
    { "4", "5",  "6",  "7",  "8" },
    { "5", "6",  "7",  "8",  "9" },
    { "6", "7",  "8",  "9",  "10" },
    { "7", "8",  "9",  "10", "11" },
    { "8", "9",  "10", "11", "12" },
    { "9", "10", "11", "12", "13" },
  };
  Canvas *tableCanvas = cw->addCanvas(tableShader, 0, 0, w, h);
  Canvas *textCanvas = cw->addCanvas(textShader, 0, 0, w, h);
  MultiShape2D *m = new MultiShape2D(tableShader);
  tableCanvas->addLayer(m);
  for (int y = 0; y < numRows; ++y) {
    for (int x = 0; x < numCols; ++x) {
      m->drawRectangle(x * tWidth + offset_x, y * tHeight + offset_y, tWidth, tHeight);
      textCanvas->addLayer(new Text(x * tWidth + offset_x + 10, y * tHeight + offset_y + 20, textShader, text[y][x]));
    }
  }



  cout << font << endl;
  //c->addLayer(new Text(100, 100, textShader, "This is TeXt"));
  //c->addLayer(new Text(100, 300, textShader, "ASOnnaeA90l!)@$9lsdf?W#KLF"));

}

void make_mouse(shared_ptr<CompiledWeb> cw) {
  cw->getParent()->inp.init();
  cw->getParent()->inp.assign_mouse("Click for mouse 1\n", 0);
  cw->getParent()->inp.start();

  uint32_t sizes[] = {12,14,16,30,40};
  FontFace::loadFonts("TIMES",sizes,5);
  const Font* font = FontFace::get("TIMES", 40, FontFace::BOLD);

  Style* cursorStyle = new Style(font, 1,0,0, 0,0,0);
  cursorStyle->setLineWidth(1);
  Canvas* c = cw->getMainCanvas();
  c->addLayer(new Cursor(20,20,cursorStyle,0));
}


int main(int argc, char *argv[]) {
  return CompiledWebWindow::initGrail(argc, argv, init);
}

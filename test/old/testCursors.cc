#include "grail/Grail.hh"

using namespace std;

void init(shared_ptr<CompiledWeb> cw) {
  const Font* font = FontFace::get("Times", 20, FontFace::BOLD);

  Style* s = new Style(font, 1, 0, 0, 0, 0, 0);
  Style* s2 = new Style(font, 0, 0, 1, 0, 0, 0);
  s->setLineWidth(1);
  s2->setLineWidth(1);
  Canvas* c = cw->getMainCanvas();
  c->addLayer(new Cursor(20, 20, s, 0));
  c->addLayer(new Cursor(200, 500, s2, 1));
}

int main(int argc, char* argv[]) {
  return CompiledWebWindow::initGrail(argc, argv, init);
}

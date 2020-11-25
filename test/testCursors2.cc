#include "grail/Grail.hh"
#include "ActionListener.hh"
#include <iostream>

using namespace std;

ActionListener *a;
void mouse_clicked_callback(ActionListener::MouseEvent e) {
  switch (e.type) {
  case ActionListener::MouseType::PRESSED:
      switch (e.data) {
      case ActionListener::MouseData::LEFT:
        std::cout << "Left pressed!\n";
        break;
      case ActionListener::MouseData::RIGHT:
        std::cout << "Right pressed!\n";
        break;
      case ActionListener::MouseData::MIDDLE:
        std::cout << "Middle pressed!\n";
        break;
      }
    break;
  case ActionListener::MouseType::RELEASED:
      switch (e.data) {
      case ActionListener::MouseData::LEFT:
        std::cout << "Left released!\n";
        break;
      case ActionListener::MouseData::RIGHT:
        std::cout << "Right released!\n";
        break;
      case ActionListener::MouseData::MIDDLE:
        std::cout << "Middle released!\n";
        break;
      }
    break;
  }
}

void init(shared_ptr<CompiledWeb> cw) {

  uint32_t sizes[] = {12,14,16,30,40};
  FontFace::loadFonts("TIMES",sizes,5);
  cw->getParent()->inp.init();
  cw->getParent()->inp.assign_mouse("Click for mouse 1\n", 0);
  cw->getParent()->inp.start();

  const Font* font = FontFace::get("TIMES", 40, FontFace::BOLD);

  Style* s = new Style(font, 1,0,0, 0,0,0);
  Style* s2 = new Style(font, 0,0,1, 0,0,0);
  s->setLineWidth(1);
  s2->setLineWidth(1);
  Canvas* c = cw->getMainCanvas();
  c->addLayer(new Cursor(20,20,s,0));

  // MultiShape2D* m = c->addLayer(new MultiShape2D(s));
  // m->drawRectangle(20,20,300,300);
  c->addLayer(new Rectangle(50, 50, 300, 300, s));
  // c->addLayer(new Cursor(200,500,s2,2));

  a = new ActionListener(&cw->getParent()->inp);
  a->registerCallback(ActionListener::Events::MOUSE, 0, mouse_clicked_callback);
}

int main(int argc, char *argv[]) {
  int ret = CompiledWebWindow::initGrail(argc, argv, init);
  delete a;
  return ret;
}

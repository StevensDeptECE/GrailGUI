#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"
#include "opengl/util/Transformation.hh"

using namespace std;
using namespace grail;

class ImageMove : public Member {
 private:
  bool hasclicked = false;
  float xpos, ypos, width, height;
  Image* piece;
  Transformation* trans;

 public:
  ImageMove(Tab* t, GLWin* w) : Member(t, 0) {
    MainCanvas* c = t->getMainCanvas();
    trans = new Transformation();

    xpos = 100;
    ypos = 100;
    width = 200;
    height = 200;

    piece = addNewImage(xpos, ypos, width, height);
    trans->ident();
    trans->rotate(0, 1, 1, 0);
  }

  Image* addNewImage(float xpos, float ypos, float width, float height) {
    Image* m = c->addLayer(
        new Image(c, xpos, ypos, width, height, "ChessTextures/brook.webp"));
    m->init();
    return m;
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Transformation Practice");
  w->setSize(1024, 800);
  new ImageMove(t, w);
}

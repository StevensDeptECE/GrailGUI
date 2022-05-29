#include "CAD/Triangle.hh"
#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class TestTriangle : public Member {
 public:
  TestTriangle(GLWin* w) : Member(w, 0, 0) {}

  void init() {
    MainCanvas* c = tab->getMainCanvas();
    const Style* s = tab->getDefaultStyle();
    Triangle* trr =
        c->addLayer(new Triangle(c, 300, 200, 500, 500, 700, 400, s));
  }
};

void grailmain(int argc, char* argv[], GLWin* w) {
  w->setTitle("Solar System");
  new TestTriangle(w);
}

#include "CAD/Circle2D.hh"
#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class TestCircle : public Member {
 public:
  TestCircle(Tab* tab) : Member(tab, 0, 0) {
    MainCanvas* c = tab->getMainCanvas();
    const Style* s = tab->getDefaultStyle();
    c->addLayer(new Circle2D(c, 678, 500, 100, 360, s));
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Circle Test");
  new TestCircle(defaultTab);
}

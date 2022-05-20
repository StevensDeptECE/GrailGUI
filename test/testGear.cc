#include "CAD/Gear.hh"
#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class TestGear : public Member {
 public:
  TestGear(Tab* tab) : Member(tab, 0, 0) {}
  void init() {
    MainCanvas* c = tab->getMainCanvas();
    const Style* s = tab->getDefaultStyle();
    c->addLayer(new Gear(c, 500.0, 500.0, 100.0, 100.0, 6.0, 100.0, 100.0, s));
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Gear Test");
  new TestGear(defaultTab);
}

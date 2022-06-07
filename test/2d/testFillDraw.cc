#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;
class TestFillDraw : public Member {
 public:
  TestFillDraw(Tab* tab) : Member(tab, -1, 0) {
    MainCanvas* c = tab->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    //gui->fillRectangle(500,600,200,200, red);
    gui->fillDrawRectangle(500, 600, 200, 200, red, black);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test StyledMultiShape drawing Filled and Edge Shapes");
  new TestFillDraw(defaultTab);
}

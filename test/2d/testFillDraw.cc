#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;
class TestFillDraw : public Member {
 public:
  TestFillDraw(Tab* tab) : Member(tab, -1, 0) {
    MainCanvas* c = tab->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();

    gui->fillDrawRectangle(300, 100, 50, 50, red, black);
    gui->fillDrawRoundRect(100, 100, 100, 100, 20, 20, black, red);
    gui->fillDrawCircle(600, 200, 50, 10, black, green);
    gui->fillDrawEllipse(800, 200, 20, 100, 10, black, orange);
    gui->fillDrawPolygon(400, 250, 100, 100, 5, black, red);
    gui->fillDrawTriangle(0, 0, 50, 50, 50, 10, green, red);
    gui->CheckeredGrid(100, 400, 400, 400, 8, 8, black, black, white);
    gui->drawGrid(500, 300, 400, 400, 8, 8, black);
    gui->drawTriGrid(0, 500, 3, 3, black);

    // gui->drawRectangle(100, 100, 100, 100, black);
    // gui->fillRectangle(300, 300, 100, 100, red);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test filldraw");
  new TestFillDraw(defaultTab);
}
#include <unistd.h>

#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class TestPolyLines : public GLWin {
 public:
  TestPolyLines() : GLWin(0x000000, 0xCCCCCC, "Test PolyLines") {}
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();

    float xy[] = {100, 0, 200, 0, 200, 200, 100, 200};
    gui->fillPolygon(xy, 4, yellow);

    float xy2[] = {300, 50, 400, 50, 400, 200, 350, 250, 300, 200};
    gui->drawPolygon(xy2, 5, pink);

    float xy3[] = {200, 500, 400, 500, 200, 600};
    gui->drawPolygon(xy3, 3, green);
    // gui->dump();
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestPolyLines()); }

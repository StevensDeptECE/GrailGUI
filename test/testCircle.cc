#include "opengl/GrailGUI.hh"
#include "CAD/Circle2D.hh"
using namespace std;
using namespace grail;

class TestCircle : public GLWin {
 public:
  TestCircle() : GLWin(0x000000, 0xCCCCCC, "Test Circle") {}
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    const Style* s = getDefaultStyle();
    Circle2D* circ = c->addLayer(new Circle2D(c, 678, 500, 100, 360, s));    
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestCircle()); }


#include "opengl/GrailGUI.hh"
#include "CAD/Gear.hh"
using namespace std;
using namespace grail;

class TestGear : public GLWin {
 public:
  TestGear() : GLWin(0x000000, 0xCCCCCC, "Test Gear") {}
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    const Style* s = getDefaultStyle();
    Gear* circ = c->addLayer(new Gear(c, 500.0, 500.0, 100.0, 100.0, 6.0, 100.0, 100.0, s));    
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestGear()); }


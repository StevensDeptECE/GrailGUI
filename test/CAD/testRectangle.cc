#include "CAD/Rectangle.hh"
#include "CAD/Triangle.hh"
#include "opengl/GrailGUI.hh"
using namespace std;
using namespace grail;

class TestRectangle : public GLWin {
 public:
  TestRectangle() : GLWin(0x000000, 0xCCCCCC, "Test Rectangle") {}
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    const Style* s = getDefaultStyle();
    Rectangle* rec = c->addLayer(new Rectangle(c, 100, 100, 100, 50, s));
    // Triangle* trr = c->addLayer(new Triangle(c, 200, 100, 150, 100, 150, 50,
    // s)); Triangle* tri = c->addLayer(new Triangle(c, 100, 100, 150, 100, 150,
    // 50, s));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestRectangle()); }

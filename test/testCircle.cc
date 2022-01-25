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
    Circle2D* circ = c->addLayer(new Circle2D(c, 10, 10, 10, 60, s));    
    //Triangle* trr = c->addLayer(new Triangle(c, 1, 1, 500, 500, 700,456, s));   
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestCircle()); }


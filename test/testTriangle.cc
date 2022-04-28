#include "opengl/GrailGUI.hh"
#include "CAD/Triangle.hh"
using namespace std;
using namespace grail;

class TestTriangle : public GLWin {
 public:
  TestTriangle() : GLWin(0x000000, 0xCCCCCC, "Test Triangle") {}
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    const Style* s = getDefaultStyle();
    //Triangle* tri = c->addLayer(new Triangle(c, 0, 50, 100, 50, s));    
    Triangle* trr = c->addLayer(new Triangle(c, 300, 200, 500, 500, 700,-400, s));   
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestTriangle()); }


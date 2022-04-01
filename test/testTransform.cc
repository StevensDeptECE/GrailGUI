#include "opengl/GrailGUI.hh"
#include "CAD/Triangle.hh"
#include "CAD/Transformation.hh"
using namespace std;
using namespace grail;

class Transform : public GLWin {
 public:
  TestTriangle() : GLWin(0x000000, 0xCCCCCC, "Test Transform") {}
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    const Style* s = getDefaultStyle();
    Transformation* translate = c->addLayer(new Transformation(10, 20, 0, new Triangle(c, 0, 50, 100, 50, s)));    
    //Triangle* trr = c->addLayer(new Triangle(c, 1, 1, 500, 500, 700,456, s));   
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestTriangle()); }

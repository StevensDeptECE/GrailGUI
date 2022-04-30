#include "opengl/GrailGUI.hh"
#include "CAD/RectangularPrism.hh"
#include "CAD/Transformation.hh"
#include <unistd.h>
#include "opengl/util/Camera.hh"
#include <numbers>
using namespace std;
using namespace grail;
using namespace std::numbers;

class TestTransform : public GLWin {
 public:
  TestTransform() : GLWin(0x000000, 0xCCCCCC, "Test Transform") {}
  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    const Style* s = getDefaultStyle();
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());
    RectangularPrism* rec = new RectangularPrism(c, s, 40, 756, 3, -10, -5, 0);
    c->addLayer(rec);
    Transformation* rotate = new Transformation(c, s, new RectangularPrism(c, s, 3, 10, 3, -10, -5, 0));
    rotate->rotate(pi/4, 1, 0, 0);
    rotate->translate(5, 4, 3);
    c->addLayer(rotate);    
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestTransform()); }

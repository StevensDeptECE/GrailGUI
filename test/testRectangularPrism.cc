#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include "CAD/RectangularPrism.hh"
#include "opengl/util/Camera.hh"
using namespace std;
using namespace grail;

class TestRectangularPrism : public GLWin {
    public:
    TestRectangularPrism() : GLWin(0x000000, 0xCCCCCC, "Test Rectangular Prism") {}
    void init() {
        MainCanvas* c = currentTab()->getMainCanvas();
        Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
        c->setProjection(cam->getViewProjection());

        const Style* s = getDefaultStyle();
        RectangularPrism* rec = new RectangularPrism(c, s, 3, 3, 3, -10, -5, 0);
        c->addLayer(rec);
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestRectangularPrism()); }
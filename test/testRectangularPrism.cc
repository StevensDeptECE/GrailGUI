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
        Camera* cam = c->setLookAtProjection(0, 0, 200, 0, 0, 0, 0, 1, 0);
        c->setProjection(cam->getViewProjection());

        const Style* s = getDefaultStyle();
        RectangularPrism* rec = c->addLayer(new RectangularPrism(c, s, 100, 200, 0, 600, 600, 100));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestRectangularPrism()); }
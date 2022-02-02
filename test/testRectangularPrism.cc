#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include "CAD/RectangularPrism.hh"
using namespace std;
using namespace grail;

class TestRectangularPrism : public GLWin {
    public:
    TestRectangularPrism() : GLWin(0x000000, 0xCCCCCC, "Test Rectangular Prism") {}
    void init() {
        MainCanvas* c = currentTab()->getMainCanvas();
        //const Style* s = getDefaultStyle();
        RectangularPrism* rec = c->addLayer(new RectangularPrism(c, 2, 5, 2, 0, 0, 0));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestRectangularPrism()); }
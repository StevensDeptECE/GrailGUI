#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include "CAD/Sphere.hh"
using namespace std;
using namespace grail;

class TestSphere : public GLWin {
    public:
    TestSphere() : GLWin(0x000000, 0xCCCCCC, "Test Sphere") {}
    void init() {
        MainCanvas* c = currentTab()->getMainCanvas();
        //const Style* s = getDefaultStyle();
        Sphere* sphere = c->addLayer(new Sphere(c, 2, 5, 2, 0, 0, 0));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestSphere()); }
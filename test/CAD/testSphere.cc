#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include "CAD/Sphere.hh"
#include "opengl/util/Camera.hh"
using namespace std;
using namespace grail;

class TestSphere : public GLWin {
    public:
    TestSphere() : GLWin(0x000000, 0xCCCCCC, "Test Sphere") {}
    void init() {
        MainCanvas* c = currentTab()->getMainCanvas();
        Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
        c->setProjection(cam->getViewProjection());

        const Style* s = getDefaultStyle();
        Sphere* sph = c->addLayer(new Sphere(c, s, 30, 30, 3));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestSphere(), 1000, 1000); }

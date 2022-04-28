#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include "CAD/Cylinder.hh"
#include "opengl/util/Camera.hh"
using namespace std;
using namespace grail;

// int main(){
//     Cylinder cylinder(2, 2, 2);
//     double volume = cylinder.getVolume();
//     cout << "Volume of Cylinder: " << volume << "\n";
//     cout << "Expected 25.13\n";

//     Cylinder cylinder(2, 2);
//     double volume = cylinder.getVolume();
//     cout << "Volume of Cylinder: " << volume << "\n";
//     cout << "Expected 25.13\n";
// }

class TestCylinder : public GLWin {
    public:
    TestCylinder() : GLWin(0x000000, 0xCCCCCC, "Test Cylinder") {}
    void init() {
        MainCanvas* c = currentTab()->getMainCanvas();
        Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
        c->setProjection(cam->getViewProjection());
        const Style* s = getDefaultStyle();
        Cylinder* cyl = c->addLayer(new Cylinder(c, s, 3, -10, -10, -10, 3, 360));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestCylinder()); }
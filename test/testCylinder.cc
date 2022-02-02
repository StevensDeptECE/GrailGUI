#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include "CAD/Cylinder.hh"
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
        //const Style* s = getDefaultStyle();
        Cylinder* cyl = c->addLayer(new Cylinder(c, 5, 0, 0, 0, 5, 360));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestCylinder()); }
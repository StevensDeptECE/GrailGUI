#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include "CAD/Sphere.hh"
using namespace std;
using namespace grail;

<<<<<<< HEAD
=======
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

>>>>>>> 644dcecb0ce6959ef8b5a74af10b4a0cdca57c9a
class TestSphere : public GLWin {
    public:
    TestSphere() : GLWin(0x000000, 0xCCCCCC, "Test Sphere") {}
    void init() {
        MainCanvas* c = currentTab()->getMainCanvas();
        //const Style* s = getDefaultStyle();
<<<<<<< HEAD
        Sphere* sphere = c->addLayer(new Sphere(c, 2, 5, 2, 0, 0, 0));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestSphere()); }
=======
        Sphere* sph = c->addLayer(new Sphere(c, 5, 5, 5));
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new TestSphere(), 1000, 1000); }
>>>>>>> 644dcecb0ce6959ef8b5a74af10b4a0cdca57c9a

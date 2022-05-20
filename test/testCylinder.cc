#include <unistd.h>
#include "opengl/GrailGUI.hh"
#include "CAD/Cylinder.hh"
#include "opengl/util/Camera.hh"
#include "CAD/Transformation.hh"
#include <numbers>

using namespace std;
using namespace grail;
using namespace std::numbers;

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

class TestCylinder : public Member {
    public:
    TestCylinder(Tab* tab) : Member(tab, 0, 0) {
        MainCanvas* c = tab->getMainCanvas();
        Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
        c->setProjection(cam->getViewProjection());
        
        const Style* s = tab->getDefaultStyle();
        // Cylinder* cyl = ;
        // c->addLayer(cyl);
        Transformation* rotate = new Transformation(c, s, new Cylinder(c, s, 4, 60, 60, 0, 1, 200));
        rotate->rotate(pi/3, 0, 2, 0);
        //rotate->translate(5, 4, 3);
        c->addLayer(rotate);    
  }
};


void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab){
  w->setTitle("Cylinder Test");
  new TestCylinder(defaultTab);
}
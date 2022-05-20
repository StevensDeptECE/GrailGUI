#include "CAD/Circle.hh"
#include "CAD/Vec3d.hh"
#include "opengl/StyledMultiShape2D.hh"
#include <iostream>
#include "opengl/GrailGUI.hh"
#include "opengl/util/Camera.hh"
#include <vector>

using namespace std;

class CircleTest : public Member {
  private:
    Style *baseGraphStyle;

  public:
    CircleTest(Tab* tab) : Member(tab,0,0) {
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);
  
    //set the camera
    MainCanvas *c = tab->getMainCanvas();
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    Vec3D a(1,1,1);
    Vec3D b(2,3,4);

    Circle* firstCircle = new Circle(1, a, b, c, baseGraphStyle);
    c->addLayer(firstCircle);

    Circle* secondCircle = new Circle(4, a, b, c, baseGraphStyle);
    c->addLayer(secondCircle);

    Circle* thirdCircle = new Circle(7, a, b, c, baseGraphStyle);
    c->addLayer(thirdCircle);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("CircleTest");
  new CircleTest(defaultTab);
}

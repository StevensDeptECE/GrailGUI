#include "CAD/Circle.hh"
#include "CAD/Vec3d.hh"
#include "opengl/StyledMultiShape2D.hh"
#include <iostream>
#include "opengl/GrailGUI.hh"
#include "opengl/util/Camera.hh"
#include <vector>

using namespace std;

class CircleTest : public GLWin {
  private:
    Style *baseGraphStyle;

  public:
    CircleTest() : GLWin(0x000000, 0xCCCCCC, "CircleTest") {}

  void init(){
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);
  
    //set the camera
    MainCanvas *c = currentTab()->getMainCanvas();
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

int main(int argc, char *argv[]) {
  return GLWin::init(new CircleTest(), 1000, 1000);
}

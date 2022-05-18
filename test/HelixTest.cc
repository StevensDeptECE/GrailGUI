#include "CAD/Helix.hh"
#include "CAD/Vec3d.hh"
#include "opengl/StyledMultiShape2D.hh"
#include <iostream>
#include "opengl/GrailGUI.hh"
#include "opengl/util/Camera.hh"

#include <vector>

using namespace std;

class HelixTest : public GLWin {
  private:
    Style *baseGraphStyle;

  public:
    HelixTest() : GLWin(0x000000, 0xCCCCCC, "HelixTest") {}

  void init(){
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);
  
    //set the camera
    MainCanvas *c = currentTab()->getMainCanvas();
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    Vec3D first(3,0,0);
    Helix* firstHelix = new Helix(first, 10, 2, c, baseGraphStyle);
    c->addLayer(firstHelix);

    Vec3D second(3,0,0);
    Helix* secondHelix = new Helix(second, 10, 4, c, baseGraphStyle);
    c->addLayer(secondHelix);

    Vec3D third(-5,0,0);
    Helix* thirdHelix = new Helix(third, 7, 1, c, baseGraphStyle);
    c->addLayer(thirdHelix);

  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new HelixTest(), 1000, 1000);
}

// #include "CAD/Helix.cc"
// #include "CAD/Circle.cc"
#include "CAD/BezierPath.hh"
#include "opengl/StyledMultiShape2D.hh"

#include <iostream>
#include "opengl/GrailGUI.hh"
#include "opengl/util/Camera.hh"

using namespace std;

class CurveTest : public GLWin {
  private:
    Style *baseGraphStyle;

  public:
    CurveTest() : GLWin(0x000000, 0xCCCCCC, "CurveTest") {}

  void init(){
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);
  
    //set the camera
    MainCanvas *c = currentTab()->getMainCanvas();
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    //rectangle test
    //StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, baseGraphStyle));
    //m->fillRectangle(0,0,100,100,grail::red);
    
    //helix test
    // Vec3D start(0,0,0);
    // Helix* test = new Helix(start, 10, 2, c, baseGraphStyle);
    // c->addLayer(test);

    //bezier test
    Vec3D a(0,0,0);
    Vec3D b(2,5,0);
    Vec3D d(5,0,0);
    Vec3D e(7,5,0);

    BezierPath* btest = new BezierPath(a,b,d,e,c,baseGraphStyle);
    btest->getPoints();
    c->addLayer(btest);
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new CurveTest(), 1000, 1000);
}

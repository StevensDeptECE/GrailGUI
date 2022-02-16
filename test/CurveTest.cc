#include "CAD/Curve.cc"
#include "CAD/Helix.cc"
#include "opengl/StyledMultiShape2D.hh"


#include "iostream"
#include "CAD/Vec3d.hh"
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
    Camera* cam = c->setLookAtProjection(20, 20, 0, 0, 0, 0, 0, 0, 1);
    c->setProjection(cam->getViewProjection());
   
    Vec3D a(0,0,0);
    Vec3D b(10,2,3);

    Curve* c1 = new Curve(a, b, c, baseGraphStyle);
    c->addLayer(c1);
    //cout << "points: " << *c1 << endl;

    //StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, baseGraphStyle));
    //m->fillRectangle(0,0,100,100,grail::red);
    
    //helix test
    Vec3D start(0,0,0);
    Helix* test = new Helix(start, 10, 2, c, baseGraphStyle);
    c->addLayer(test);
    //c1.init();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new CurveTest(), 1000, 1000);
}
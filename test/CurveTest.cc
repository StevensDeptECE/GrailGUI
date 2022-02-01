#include "CAD/Curve.cc"
#include "CAD/Helix.cc"

#include "iostream"
#include "CAD/Vec3d.hh"
#include "opengl/GrailGUI.hh"
using namespace std;

class CurveTest : public GLWin {
  private:
    Style *baseGraphStyle;

  public:
    CurveTest() : GLWin(0x000000, 0xCCCCCC, "CurveTest") {}

  void init(){
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);
  
    MainCanvas *c = currentTab()->getMainCanvas();
    
    Vec3D a(1,1,1);
    Vec3D b(200,300,400);

    Curve* c1 = new Curve(a, b, c, baseGraphStyle);
    //c->addLayer(c1);
    //cout << "points: " << *c1 << endl;

    //helix test
    Vec3D start(100,100,0);
    Helix* test = new Helix(start, 100, 50, c, baseGraphStyle);
    c->addLayer(test);
    //c1.init();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new CurveTest(), 1000, 1000);
}
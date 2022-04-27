#include "CAD/Facet.hh"
#include "CAD/Vec3d.hh"
#include "opengl/StyledMultiShape2D.hh"
#include <iostream>
#include "opengl/GrailGUI.hh"
#include "opengl/util/Camera.hh"
#include <vector>

using namespace std;

class FacetTest : public GLWin {
  private:
    Style *baseGraphStyle;

  public:
    FacetTest() : GLWin(0x000000, 0xCCCCCC, "FacetTest") {}

  void init(){
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);
  
    //set the camera
    MainCanvas *c = currentTab()->getMainCanvas();
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    std::vector<Vec3D> points;

    Vec3D f1(3,0,0);
    Vec3D f2(3,3,0);
    Vec3D f3(6,0,0);
    Vec3D f4(6,3,0);
    points.push_back(f1);
    points.push_back(f2);
    points.push_back(f4);
    points.push_back(f3);
    Facet* firstFacet = new Facet(points,c,baseGraphStyle);
    c->addLayer(firstFacet);

  for (int i=0; i<points.size(); i++){
     points[i] = points[i]+ Vec3D(-10,-10,-10);
  }
  points[2]=points[3];
  points.erase(points.begin() + 3);
  Facet* secondFacet = new Facet(points,c,baseGraphStyle);
  c->addLayer(secondFacet);

    Vec3D f5(3,-5,0);
    Vec3D f6(6,-3,0);
    Vec3D f7(9,-5,0);
    Vec3D f8(8,-9,0);
    Vec3D f9(4,-9,0);

    points.clear();
    points.push_back(f5);
    points.push_back(f6);
    points.push_back(f7);
    points.push_back(f8);
    points.push_back(f9);

    Facet* thirdFacet = new Facet(points,c,baseGraphStyle);
    c->addLayer(thirdFacet);
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new FacetTest(), 1000, 1000);
}

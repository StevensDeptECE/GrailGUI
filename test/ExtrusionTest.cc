#include "CAD/Helix.hh"
#include "CAD/Triangle.hh"
#include "CAD/Circle.hh"
#include "CAD/Line.hh"
#include "CAD/Facet.hh"
#include "CAD/Extrusion.hh"


#include "CAD/Vec3d.hh"

#include "CAD/BezierPath.hh"
#include "opengl/StyledMultiShape2D.hh"

#include <iostream>
#include "opengl/GrailGUI.hh"
#include "opengl/util/Camera.hh"

#include <vector>

using namespace std;

class ExtrusionTest : public GLWin {
  private:
    Style *baseGraphStyle;

  public:
    ExtrusionTest() : GLWin(0x000000, 0xCCCCCC, "ExtrusionTest") {}

  void init(){
    //set
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(5);
  
    //set the camera
    MainCanvas *c = currentTab()->getMainCanvas();
    //set x to -80 to get a view of the x,z plane
    Camera* cam = c->setLookAtProjection(0, 0, 75, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());   

    std::vector<Vec3D> facetPoints;
    Vec3D f1(3,0,0);
    Vec3D f2(3,3,0);
    Vec3D f3(6,0,0);
    Vec3D f4(6,3,0);

    facetPoints.push_back(f1);
    facetPoints.push_back(f2);
    facetPoints.push_back(f4);
    facetPoints.push_back(f3);

    //Facet* ftest = new Facet(points,c,baseGraphStyle);
    //c->addLayer(ftest);

    //create a bezier curve
    Vec3D b1(3,0,0);
    Vec3D b2(6,7,0);
    Vec3D b3(9,0,0);
    Vec3D b4(12,7,0);
    BezierPath* bez = new BezierPath(b1,b2,b3,b4,c,baseGraphStyle);
    c->addLayer(bez);
    Extrusion* firstExtrusion = new Extrusion(facetPoints,bez->points,c, baseGraphStyle); 


    //helix test
    // Vec3D start(-3,0,0);
    // Helix* hel = new Helix(start, 3, 2, c, baseGraphStyle);

    Vec3D b5(-20,-20,0);
    Vec3D b6(-15,-10,0);
    Vec3D b7(-10,-20,0);
    Vec3D b8(-5,-10,0);
    BezierPath* bez2 = new BezierPath(b5,b6,b7,b8,c,baseGraphStyle);
    c->addLayer(bez2);
    Vec3D f5(-20,-20,0);
    Vec3D f6(-20,-17,0);
    Vec3D f7(-17,-20,0);
    facetPoints.clear();
    facetPoints.push_back(f5);
    facetPoints.push_back(f6);
    facetPoints.push_back(f7);
    cout << bez2->points[0] << endl;
    Extrusion* secondExtrusion = new Extrusion(facetPoints,bez2->points,c, baseGraphStyle); 
  
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new ExtrusionTest(), 1000, 1000);
}

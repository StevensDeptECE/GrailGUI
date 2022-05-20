#include <iostream>
#include <vector>

#include "CAD/BezierPath.hh"
#include "CAD/Circle.hh"
#include "CAD/Extrusion.hh"
#include "CAD/Facet.hh"
#include "CAD/Helix.hh"
#include "CAD/Line.hh"
#include "CAD/Triangle.hh"
#include "CAD/Vec3d.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/util/Camera.hh"

using namespace std;

class CurveTest : public Member {
 private:
  Style* baseGraphStyle;

 public:
  CurveTest(Tab* tab) : Member(tab, 0, 0) {}

  void init() {
    // set
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);

    // set the camera
    MainCanvas* c = tab->getMainCanvas();
    // set x to -80 to get a view of the x,z plane
    // Camera* cam = c->setLookAtProjection(-80, 0, 100, 0, 0, 0, 0, 1, 0);
    // c->setProjection(cam->getViewProjection());
    // cout<< "HELLO" << endl;
    std::vector<Vec3D> facetPoints;
    Vec3D f1(3, 0, 0);
    Vec3D f2(3, 3, 0);
    Vec3D f3(6, 0, 0);
    Vec3D f4(6, 3, 0);

    facetPoints.push_back(f1);
    facetPoints.push_back(f2);
    facetPoints.push_back(f4);
    facetPoints.push_back(f3);

    // Facet* ftest = new Facet(points,c,baseGraphStyle);
    // c->addLayer(ftest);

    // create a bezier curve
    Vec3D b1(3, 0, 0);
    Vec3D b2(6, 7, 0);
    Vec3D b3(9, 0, 0);
    Vec3D b4(12, 7, 0);
    BezierPath* bez = new BezierPath(b1, b2, b3, b4, c, baseGraphStyle);

    Extrusion* firstExtrusion =
        new Extrusion(facetPoints, bez->points, c, baseGraphStyle);

    c->addLayer(firstExtrusion);
    // helix test
    Vec3D start(-3, 0, 0);
    Helix* hel = new Helix(start, 10, 2, c, baseGraphStyle);

    Vec3D f5(-3, 0, 0);
    Vec3D f6(-3, 3, 0);
    Vec3D f7(0, 0, 0);
    facetPoints.clear();
    facetPoints.push_back(f5);
    facetPoints.push_back(f6);
    facetPoints.push_back(f7);

    Extrusion* secondExtrusion =
        new Extrusion(facetPoints, hel->points, c, baseGraphStyle);

    c->addLayer(secondExtrusion);
    // Vec3D start(0,0,0);
    // Helix* test = new Helix(start, 10, 2, c, baseGraphStyle);
    // c->addLayer(test);

    // bezier test
    //  Vec3D a(0,0,0);
    // Vec3D b(2,5,0);
    // Vec3D d(5,0,0);
    // Vec3D e(7,5,0);

    // BezierPath* btest = new BezierPath(a,b,d,e,c,baseGraphStyle);
    // c->addLayer(btest);
    // cout << "new curve test" << endl;
    // Line* ltest = new Line(a,b,c,baseGraphStyle);
    // c->addLayer(ltest);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Curve Test");
  new CurveTest(defaultTab);
}

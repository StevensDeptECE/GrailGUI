#include <iostream>
#include <vector>

#include "CAD/Facet.hh"
#include "CAD/Vec3d.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/util/Camera.hh"

using namespace std;

class FacetTest : public Member {
 private:
  Style* baseGraphStyle;

 public:
  FacetTest(Tab* tab) : Member(tab, 0, 0) {
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);

    // set the camera
    MainCanvas* c = tab->getMainCanvas();
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    std::vector<Vec3D> points;

    Vec3D f1(3, 0, 0);
    Vec3D f2(3, 3, 0);
    Vec3D f3(6, 0, 0);
    Vec3D f4(6, 3, 0);
    points.push_back(f1);
    points.push_back(f2);
    points.push_back(f4);
    points.push_back(f3);
    Facet* firstFacet = new Facet(points, c, baseGraphStyle);
    c->addLayer(firstFacet);

    for (int i = 0; i < points.size(); i++) {
      points[i] = points[i] + Vec3D(-10, -10, -10);
    }
    points[2] = points[3];
    points.erase(points.begin() + 3);
    Facet* secondFacet = new Facet(points, c, baseGraphStyle);
    c->addLayer(secondFacet);

    Vec3D f5(3, -5, 0);
    Vec3D f6(6, -3, 0);
    Vec3D f7(9, -5, 0);
    Vec3D f8(8, -9, 0);
    Vec3D f9(4, -9, 0);

    points.clear();
    points.push_back(f5);
    points.push_back(f6);
    points.push_back(f7);
    points.push_back(f8);
    points.push_back(f9);

    Facet* thirdFacet = new Facet(points, c, baseGraphStyle);
    c->addLayer(thirdFacet);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Facet Test");
  new FacetTest(defaultTab);
}

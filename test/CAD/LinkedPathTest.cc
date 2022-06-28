#include <iostream>
#include <vector>

#include "CAD/BezierPath.hh"
#include "CAD/Helix.hh"
#include "CAD/Line.hh"
#include "CAD/Vec3d.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/util/Camera.hh"

using namespace std;

class LineTest : public Member {
 private:
  Style* baseGraphStyle;

 public:
  LineTest(Tab* tab) : Member(tab, 0, 0) {
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);

    // set the camera
    MainCanvas* c = tab->getMainCanvas();
    Camera* cam = c->setLookAtProjection(0, 0, 75, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    Vec3D start1(-10, 0, 0);
    Vec3D stop1(0, 0, 0);
    Seg* firstLine = new Seg(start1, stop1, c, baseGraphStyle);
    c->addLayer(firstLine);

    Vec3D b2(3, 7, 0);
    Vec3D b3(6, 0, 0);
    Vec3D b4(9, 7, 0);
    BezierPath* firstBezier =
        new BezierPath(stop1, b2, b3, b4, c, baseGraphStyle);
    c->addLayer(firstBezier);

    Vec3D stop2(15, 0, 0);
    Seg* secondLine = new Seg(b4, stop2, c, baseGraphStyle);
    c->addLayer(secondLine);

    Helix* secondHelix = new Helix(b4, 10, 4, c, baseGraphStyle);
    c->addLayer(secondHelix);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Linked Path Test");
  new LineTest(defaultTab);
}

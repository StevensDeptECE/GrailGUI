#include <iostream>
#include <vector>

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
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    Vec3D start1(3, 0, 0);
    Vec3D stop1(3, 7, 0);
    Line* firstLine = new Line(start1, stop1, c, baseGraphStyle);
    c->addLayer(firstLine);

    Vec3D start2(0, 0, 0);
    Vec3D stop2(-6, -15, 0);
    Line* secondLine = new Line(start2, stop2, c, baseGraphStyle);
    c->addLayer(secondLine);

    Vec3D start3(-3, 10, 0);
    Vec3D stop3(0, 0, 0);
    Line* thirdLine = new Line(start3, stop3, c, baseGraphStyle);
    c->addLayer(thirdLine);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Line Test");
  new LineTest(defaultTab);
}
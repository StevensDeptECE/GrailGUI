#include <iostream>
#include <vector>

#include "CAD/Helix.hh"
#include "CAD/Vec3d.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/util/Camera.hh"

using namespace std;

class HelixTest : public Member {
 private:
  Style* baseGraphStyle;

 public:
  HelixTest(Tab* tab) : Member(tab, 0, 0) {
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);

    // set the camera
    MainCanvas* c = tab->getMainCanvas();
    Camera* cam = c->setLookAtProjection(0, 0, 30, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    Vec3D first(3, 0, 0);
    Helix* firstHelix = new Helix(first, 10, 2, c, baseGraphStyle);
    c->addLayer(firstHelix);

    Vec3D second(3, 0, 0);
    Helix* secondHelix = new Helix(second, 10, 4, c, baseGraphStyle);
    c->addLayer(secondHelix);

    Vec3D third(-5, 0, 0);
    Helix* thirdHelix = new Helix(third, 7, 1, c, baseGraphStyle);
    c->addLayer(thirdHelix);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Helix Test");
  new HelixTest(defaultTab);
}

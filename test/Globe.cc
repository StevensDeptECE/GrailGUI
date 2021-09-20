#include "opengl/GrailGUI.hh"
#include "opengl/MultiShape3D.hh"
#include "opengl/util/Transformation.hh"
#include "util/Ex.hh"

using namespace std;

class Globe : public Member {
 private:
  Transformation tEarth;
  float earthRotationAngle;

 public:
  Globe(Tab* tab) : Member(tab, 60, 0.1) {
    earthRotationAngle = .01;
    Canvas* c = tab->getMainCanvas();
    Camera* cam = c->setLookAtProjection(2, 3, 40, 0, 0, 0, 0, 0, 1);
    MultiShape3D* earth =
        c->addLayer(new MultiShape3D(c, cam, "textures/earth.jpg", &tEarth));
    earth->genOBJModel("models/sphere.obj");
  }
  void update() {
    tEarth.setRotate(-23.5f * DEG2RAD<float>, 0.0f, 0.0f,
                     1.0f);  // rotate axis by 23.5 degrees
    tEarth.rotate((float)(earthRotationAngle * tab->time()), 0.0f, 1.0f, 0.0f);
    tEarth.scale(5);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Solar System");
  new Globe(defaultTab);
}

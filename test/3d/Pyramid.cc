#include "opengl/GrailGUI.hh"
#include "opengl/MultiShape3D.hh"
#include "opengl/util/Transformation.hh"
#include "CAD/Triangle.hh"

using namespace std;

class Pyramid : public Member {
 private:
  Transformation tPyramid;
  float earthRotationAngle;

 public:
 typedef std::vector<std::pair<float, float>> TexCoordVector;
  Pyramid(Tab* tab) : Member(tab, 0, 0.1) {
    earthRotationAngle = .01;
    Canvas* c = tab->getMainCanvas();
    Camera* cam = c->setLookAtProjection(2, 3, 40, 0, 0, 0, 0, 0, 1);
    MultiShape3D* earth =
        c->addLayer(new MultiShape3D(c, cam, "textures/Pyramid.png", &tPyramid));
    earth->genPyramid(0, 0, 0, 1, 1, 1, TexCoordVector);
  }
  void update() {
    tPyramid.setRotate(-23.5f * DEG2RAD<float>, 0.0f, 0.0f,
                     1.0f);  // rotate axis by 23.5 degrees
    tPyramid.rotate((float)(earthRotationAngle * tab->time()), 0.0f, 1.0f, 0.0f);
    tPyramid.scale(5);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Pyramid");
  new Pyramid(defaultTab);
}

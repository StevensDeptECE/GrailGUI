#include "CAD/Transform.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MultiShape3D.hh"
#include "opengl/util/Transformation.hh"

using namespace std;

class Globe : public Member {
 private:
  Transformation tEarth;
  float earthRotationAngle;

 public:
  Globe(Tab* tab) : Member(tab, 0, 0.1) {
    earthRotationAngle = .01;
    Canvas* c = tab->getMainCanvas();

    Camera* cam = c->setLookAtProjection(
        2, 3, 40, 0, 0, 0, 0, 0, 1);  // TODO: these 2 should be one operation!
    c->setProjection(glm::scale(
        cam->getViewProjection(),
        glm::vec3(0.5, 0.5, 0.5)));  // set default projection for the canvas to
                                     // be 3d looking from (2,3,40) to (0,0,0)
                                     // with (0,0,1) is "up"

    MultiShape3D* earth =
        new MultiShape3D(c, cam, "textures/earth.jpg", &tEarth);
    earth->genOBJModel("models/sphere.obj");

    Transform* t;
    c->addLayer(t = new Transform(c, earth));
    t->translate(20, 0, 0);
    // c->addLayer(earth);

    c->addLayer(t = new Transform(c, earth));
    t->translate(-30, 0, 0);
  }

  void update() {
    tEarth.setRotate(-23.5f * DEG2RAD<float>, 0.0f, 0.0f,
                     1.0f);  // rotate axis by 23.5 degrees
    tEarth.rotate((float)(earthRotationAngle * tab->time()), 0.0f, 1.0f, 0.0f);
    tEarth.scale(5);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Globe");
  new Globe(defaultTab);
}

#include "grail/Grail.hh"
#include "opengl/GLMath.hh"
#include "opengl/util/Transformation.hh"
#include "openglColors.hh"
using namespace std;

class TestDemo3D : public GLWin {
 private:
  Camera* cam;
  Transformation tEarth;
  Transformation tMoon;
  Transformation tShip;
  Transformation tSky;

  float moonOrbit;
  float earthRot;

  float shipx, shipy, shipz;
  float t;
  float dt;
  static constexpr float s = 0.3;

 public:
  ~TestDemo3D() { delete cam; }

  static void forewardFly(GLWin* w) {
    TestDemo3D* d = (TestDemo3D*)(w);
    d->shipx -= s, d->shipy += s, d->shipz += s;
  }
  static void backFly(GLWin* w) {
    TestDemo3D* d = (TestDemo3D*)(w);
    d->shipx += s, d->shipy -= s, d->shipz -= s;
  }

  static void lookUp(GLWin* w) {
    ((TestDemo3D*)w)->cam->incrLookingAt(glm::vec3(0, 1, 0));
  }
  static void lookDown(GLWin* w) {
    ((TestDemo3D*)w)->cam->incrLookingAt(glm::vec3(0, -1, 0));
  }
  static void lookLeft(GLWin* w) {
    ((TestDemo3D*)w)->cam->incrLookingAt(glm::vec3(-1, 0, 0));
  }
  static void lookRight(GLWin* w) {
    ((TestDemo3D*)w)->cam->incrLookingAt(glm::vec3(1, 0, 0));
  }

  void init() {
    t = 0;
    dt = 0.01;
    earthRot = 1;
    moonOrbit = 0.05;
    Style* s = getDefaultStyle();
    Canvas* c = currentTab()->getMainCanvas();

    cam = new Camera(width, height);
    cam->setCamPos(glm::vec3(12, 12, 12));
    cam->setLookingAt(glm::vec3(0, 0, 0));

    MultiShape3D* earth =
        c->addLayer(new MultiShape3D(s, cam, "textures/earth.tga", &tEarth));
    earth->genOBJModel("models/earth.obj");

    MultiShape3D* moon =
        c->addLayer(new MultiShape3D(s, cam, "textures/moon.jpg", &tMoon));
    moon->genOBJModel("models/moon.obj");

    MultiShape3D* ship =
        c->addLayer(new MultiShape3D(s, cam, "textures/starship.jpg", &tShip));
    ship->genOBJModel("models/starship.obj");
    shipx = 10, shipy = 4, shipz = 0;

    tSky.ident();
    tSky.translate(0, 0, -10);
    MultiShape3D* sky =
        c->addLayer(new MultiShape3D(s, cam, "textures/sky1.jpg", &tSky));
    sky->genOBJModel("models/spacesky.obj");

    // Arrow keys move ship, wasd look camera around
    setAction(1000, forewardFly);
    setAction(1001, backFly);
    setAction(1002, lookUp);
    setAction(1003, lookDown);
    setAction(1004, lookLeft);
    setAction(1005, lookRight);

    setEvent(262, 1000);
    setEvent(263, 1001);
    setEvent(87, 1002);
    setEvent(83, 1003);
    setEvent(65, 1004);
    setEvent(68, 1005);
  }
  void update() {
    tEarth.ident();
    tEarth.rotate(23.5 * DEG2RAD<float>, 0, 0, 1);
    tEarth.rotate(earthRot * t, 0, 1, 0);
    tEarth.scale(2);

    tMoon.ident();
    tMoon.rotate(moonOrbit * t, 0, 1, 0);
    tMoon.translate(8, 0, 0);
    tMoon.scale(0.5);

    // Starship
    tShip.ident();
    tShip.translate(shipx, shipy, shipz);
    tShip.rotate(0.4, -1, 1, 1);
    tShip.scale(0.001);
    t += dt;
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestDemo3D(), 1600, 900);
}

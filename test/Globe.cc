#include "opengl/GrailGUI.hh"
#include "opengl/MultiShape3D.hh"
#include "opengl/util/Transformation.hh"
using namespace std;

class Globe : public GLWin {
 private:
  Transformation tEarth;
  float earthRotationAngle;

 public:
  void init() {
    earthRotationAngle = .01;
    Canvas *c = currentTab()->getMainCanvas();
    Camera *cam = c->setLookAtProjection(2, 3, 40, 0, 0, 0, 0, 0, 1);
    MultiShape3D *earth =
        c->addLayer(new MultiShape3D(c, cam, "textures/earth.jpg", &tEarth));
    earth->genOBJModel("models/sphere.obj");
  }
  void update() {
    currentTab()->getMainCanvas()->getWin()->setDirty();
    tEarth.setRotate(-23.5f * DEG2RAD<float>, 0.0f, 0.0f,
                     1.0f);  // rotate axis by 23.5 degrees
    tEarth.rotate((float)(earthRotationAngle * time()), 0.0f, 1.0f, 0.0f);
    tEarth.scale(5);
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new Globe(), 2048, 2048);
}

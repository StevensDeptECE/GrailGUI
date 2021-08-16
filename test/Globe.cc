#include "opengl/Animated.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MultiShape3D.hh"
#include "opengl/util/Transformation.hh"
#include "util/Ex.hh"

using namespace std;

class Globe : public Animated {
 private:
  Transformation tEarth;
  float earthRotationAngle;

 public:
  Globe(Tab* tab) : Animated(tab) {
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

void grailmain(int argc, char* argv[], GLWin* w, Tab* tab) {
  Canvas* c = tab->getMainCanvas();
  tab->setFrameRate(60);
  tab->setDt(0.0001);
  tab->addAnimated(new Globe(tab));
}

// TODO: replace with Main.cc once serverside is stable
int main(int argc, char* argv[]) {
  try {
    GLWin w(1024, 800, 0xFFFFFFFF, 0x000000FF, "Grail Window");
    Tab* tab = w.currentTab();
    grailmain(argc, argv, &w, tab);
    w.mainLoop();
    // g->t = thread(crun, g);
    // TODO: move this to GLWin::cleanup or destructor?  FontFace::emptyFaces();
    return 0;
  } catch (const Ex& e) {
    cerr << e << '\n';
  } catch (const char* msg) {
    cerr << msg << endl;
  } catch (const std::exception& e) {
    cerr << e.what() << endl;
  } catch (...) {
    cerr << "uncaught exception! (ouch)\n";
  }
  return 1;  // if exception caught return error
}
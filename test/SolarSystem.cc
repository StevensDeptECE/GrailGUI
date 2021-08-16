#include <cmath>
#include <numbers>
#include <string>

#include "opengl/Errcode.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MultiShape3D.hh"
#include "opengl/util/Transformation.hh"
#include "util/DynArray.hh"
#include "util/Ex.hh"
using namespace std;

class Body {
 private:
  std::string name;  // for label, if desired
  double r;  // radius in m, for OpenGL you will have to scale, float can't
             // handle space well
  double orbitalRadius;  // distance of circular orbit
  double orbitalFreq;   // inverse of orbital Period, used for relative speed of
                        // animation
  double rotationFreq;  // inverse of rotation Period
  double axialTilt;     // TODO: this should be a full vector, and if you want
                        // accuracy, precession...
  double phase;
  Transformation t;
#if 0
  Vec3d pos; // position
  Vec3d v;   // velocity
  Vec3d a;   // acceleration
#endif
 public:
  Body(Canvas* c, const Style* s, Camera* cam, const std::string& name,
       const char textureFile[], double r, double orbitalRadius,
       double orbitalPeriod, double rotationalPeriod, double axialTilt);
  void update(double time);
};

Body::Body(Canvas* c, const Style* s, Camera* cam, const std::string& name,
           const char textureFile[], double r, double orbitalRadius,
           double orbitalPeriod, double rotationalPeriod, double axialTilt)
    : name(name),
      r(r),
      orbitalRadius(orbitalRadius),
      orbitalFreq(1.0 / orbitalPeriod),
      rotationFreq(1.0 / rotationalPeriod) {
  phase = numbers::pi;
  MultiShape3D* body = c->addLayer(new MultiShape3D(c, cam, textureFile, &t));
  body->genOBJModel("models/sphere.obj");
  t.ident();
  t.scale(r);
}

void Body::update(double time) {
  t.ident();  // set to the identity transformation
  cout << "name= " << name << " orbitalFreq=" << orbitalFreq * time << '\n';
  t.rotate(orbitalFreq * time, 0, 1, 0);
  t.translate(orbitalRadius, 0, 0);
  t.rotate(-orbitalFreq * time, 0, 1, 0);
  t.rotate((float)(axialTilt * DEG2RAD<double>), 0.0f, 0.0f,
           1.0f);  // TODO: fix general axial tilt
  t.rotate((float)(rotationFreq * time), 0.0f, 1.0f, 0.0f);
  t.scale(r);  // scale to the relative size of this body
  cout << t << '\n';
}

// TODO; Implement polar coordinates???
// TODO: Implement panUp/Down for 2D-like views
// TODO: Follow planets

class SolarSystem {
 private:
  Camera* cam;
  Tab* tab;
  MainCanvas* c;
  Transformation tSky;
  Transformation tEarth;
  Transformation tSun;
  Transformation tMoon;
  Transformation tJupiter;

  float earthOrbitAngle;
  float earthRotationAngle;
  float moonOrbitAngle;

  float jupiterOrbitFreq;
  float jupiterRotationFreq;
  DynArray<Body> bodies;  // list of all bodies to be drawn

 public:
  SolarSystem(Tab* tab) : tab(tab), bodies(10) {
    earthOrbitAngle = 1.0 / 365.2425;
    earthRotationAngle = 0.9972;  // Earth rotates in 23h 56m 4.1s.
    moonOrbitAngle = 1.0 / 28.5;  // moon orbits about once every 28.5 days,
                                  // depending on the way you measure

    jupiterOrbitFreq = earthOrbitAngle / 12;  // 12 of our earth years
    jupiterRotationFreq = 0.45;               // 10 hours-ish?
    c = tab->getMainCanvas();
    cam = c->setLookAtProjection(2, 3, 40, 0, 0, 0, 0, 0, 1);
    GLWin* w = tab->getParentWin();
    const Style* s = w->getDefaultStyle();
    const Font* font = w->getDefaultFont();
    defineBindings();
    MultiShape3D* sky = c->addLayer(new MultiShape3D(
        c, cam, "textures/sky1.png", &tSky, "models/spacesky.obj"));
    // tSky.translate(0,0,-10);
    // tSky.scale(10);

    MultiShape3D* earth = c->addLayer(new MultiShape3D(
        c, cam, "textures/earth.jpg", &tEarth, "models/sphere.obj"));
    MultiShape3D* sun = c->addLayer(new MultiShape3D(
        c, cam, "textures/sun.jpg", &tSun, "models/sphere.obj"));
    MultiShape3D* moon = c->addLayer(new MultiShape3D(
        c, cam, "textures/moon.jpg", &tMoon, "models/sphere.obj"));
    MultiShape3D* jupiter = c->addLayer(new MultiShape3D(
        c, cam, "textures/jupiter.jpg", &tJupiter, "models/sphere.obj"));
#if 0
    bodies.add(Body(c, s, cam, "Earth", "textures/earth.jpg", 5, 3, 365.2425,
                    0.996, 23.5));
    bodies.add(
        Body(c, s, cam, "Mars", "textures/mars.jpg", 0.2, 11, 687, 1.14, 0));
    bodies.add(Body(c, s, cam, "Earth", "textures/earth.jpg", 1, 14, 365.2425,
                    0.996, 23.5));
    bodies.add(Body(c, s, cam, "Jupiter", "textures/jupiter.jpg", 1.5, 0,
                    12 * 365.2425, 0.48, 0));
#endif
    update();
  }

  void panBack();
  void panForward();
  void panRight();
  void panLeft();

  void update() {
    double t = tab->time();
    //#if 0
    tEarth.ident();  // set to the identity transformation
    tEarth.rotate(earthOrbitAngle * t, 0, 1, 0);
    tEarth.translate(8, 0, 0);
    tEarth.rotate(-earthOrbitAngle * t, 0, 1, 0);
    tMoon = tEarth;
    tEarth.rotate(23.5f * DEG2RAD<float>, 0.0f, 0.0f,
                  1.0f);  // rotate axis by 23.5 degrees
    tEarth.rotate((float)(earthRotationAngle * t), 0.0f, 1.0f, 0.0f);
    tEarth.scale(0.4);

    tMoon.rotate(6.8f * DEG2RAD<float>, 0.0f, 0.0f, 1.0f);
    tMoon.rotate(moonOrbitAngle * t, 0, 1, 0);
    tMoon.translate(2, 0, 0);
    tMoon.scale(0.15);

    tSun.ident();
    tSun.rotate(earthRotationAngle * .045 * t, 0, 1, 0);
    tSun.scale(1.5);
    //#endif

    tJupiter.ident();  // set to the identity transformation
    // tEarth.scale(2);
    tJupiter.rotate(jupiterOrbitFreq * t, 0, 1, 0);
    tJupiter.translate(14, 0, 0);
    tJupiter.rotate(-jupiterOrbitFreq * t, 0, 1, 0);
    tJupiter.rotate((float)(jupiterRotationFreq * t), 0.0f, 1.0f, 0.0f);
    tJupiter.scale(1.5);
  }
  void defineBindings();
};

void SolarSystem::panBack() { cam->translate(0, 0, +1); }
void SolarSystem::panForward() { cam->translate(0, 0, -1); }
void SolarSystem::panRight() { cam->translate(+1, 0, 0); }
void SolarSystem::panLeft() { cam->translate(-1, 0, 0); }

void SolarSystem::defineBindings() {
  tab->bindEvent(Tab::Inputs::RARROW, &SolarSystem::panRight, this);
  tab->bindEvent(Tab::Inputs::LARROW, &SolarSystem::panLeft, this);
  tab->bindEvent(Tab::Inputs::UPARROW, &SolarSystem::panForward, this);
  tab->bindEvent(Tab::Inputs::DOWNARROW, &SolarSystem::panBack, this);
}

void grailmain(int argc, char* argv[], GLWin* w) {
  Tab* tab = w->currentTab();
  Canvas* c = tab->getMainCanvas();
  tab->setFrameRate(60);
  tab->setDt(0.0001);
  SolarSystem solar(tab);
}

int main(int argc, char* argv[]) {
  try {
    GLWin w(1024, 800, 0xFFFFFFFF, 0x000000FF, "Grail Window");
    grailmain(argc, argv, &w);
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

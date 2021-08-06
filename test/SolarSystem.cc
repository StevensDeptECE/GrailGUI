#define _USE_MATH_DEFINES
//#include <numbers> For C++ 20 constants
#include <cmath>
#include <string>
#include <vector>

#include "opengl/Errcode.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MultiShape3D.hh"
#include "opengl/util/Transformation.hh"
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
  Body(Canvas* c, Style* s, Camera* cam, const std::string& name,
       const char textureFile[], double r, double orbitalRadius,
       double orbitalPeriod, double rotationalPeriod, double axialTilt);
  void update(double time);
};

Body::Body(Canvas* c, Style* s, Camera* cam, const std::string& name,
           const char textureFile[], double r, double orbitalRadius,
           double orbitalPeriod, double rotationalPeriod, double axialTilt)
    : name(name),
      r(r),
      orbitalRadius(orbitalRadius),
      orbitalFreq(1.0 / orbitalPeriod),
      rotationFreq(1.0 / rotationalPeriod) {
  phase = M_PI;
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

class SolarSystem : public GLWin {
 private:
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
  vector<Body> bodies;  // list of all bodies to be drawn
  Camera* cam;

 public:
  // TODO; Implement polar coordinates???
  // TODO: Implement panUp/Down for 2D-like views
  // TODO: Follow planets
  void panBack() { cam->translate(0, 0, +1); }
  void panForward() { cam->translate(0, 0, -1); }
  void panRight() { cam->translate(+1, 0, 0); }
  void panLeft() { cam->translate(-1, 0, 0); }

  void init() {
    setUpdateTime(1.0 / 60);
    setDt(0.1);
    earthOrbitAngle = 1.0 / 365.2425;
    earthRotationAngle = 0.9972;  // Earth rotates in 23h 56m 4.1s.
    moonOrbitAngle = 1.0 / 28.5;  // moon orbits about once every 28.5 days,
                                  // depending on the way you measure

    jupiterOrbitFreq = earthOrbitAngle / 12;  // 12 of our earth years
    jupiterRotationFreq = 0.45;               // 10 hours-ish?

    const Style* s = getDefaultStyle();
    const Font* font = getDefaultFont();
    Canvas* c = currentTab()->getMainCanvas();
    cam = c->setLookAtProjection(2, 3, 40, 0, 0, 0, 0, 0, 1);
    //#if 0
    MultiShape3D* sky =
        c->addLayer(new MultiShape3D(c, cam, "textures/sky1.png", &tSky));
    sky->genOBJModel("models/spacesky.obj");
    // tSky.translate(0,0,-10);
    // tSky.scale(10);

    MultiShape3D* earth =
        c->addLayer(new MultiShape3D(c, cam, "textures/earth.jpg", &tEarth));
    earth->genOBJModel("models/sphere.obj");

    MultiShape3D* sun =
        c->addLayer(new MultiShape3D(c, cam, "textures/sun.jpg", &tSun));
    sun->genOBJModel("models/sphere.obj");

    MultiShape3D* moon =
        c->addLayer(new MultiShape3D(c, cam, "textures/moon.jpg", &tMoon));
    moon->genOBJModel("models/sphere.obj");
    //#endif
    //    bodies.push_back(Body(s, c, cam, "Earth", "textures/earth.jpg", 5, 3,
    //    365.2425, 0.996, 23.5 )); bodies.push_back(Body(s, c, cam, "Mars",
    //    "textures/mars.jpg", 0.2, 11, 687, 1.14, 0)); bodies.push_back(Body(s,
    //    c, cam, "Earth", "textures/earth.jpg", 1, 14, 365.2425, 0.996, 23.5
    //    )); bodies.push_back(Body(s, c, cam, "Jupiter",
    //    "textures/jupiter.jpg", 1.5, 0, 12*365.2425, 0.48, 0 ));
    MultiShape3D* jupiter = c->addLayer(
        new MultiShape3D(c, cam, "textures/jupiter.jpg", &tJupiter));
    jupiter->genOBJModel("models/sphere.obj");

    // right arrow = pan right
    // left arrow = pan left
    // page up = zoom out
    // page down = zoom in
    bindEvent(Inputs::RARROW, &SolarSystem::panRight, this);
    bindEvent(Inputs::LARROW, &SolarSystem::panLeft, this);
    bindEvent(Inputs::UPARROW, &SolarSystem::panForward, this);
    bindEvent(Inputs::DOWNARROW, &SolarSystem::panBack, this);
    bindEvent(Inputs::INSERT, &SolarSystem::speedTime, this);
    bindEvent(Inputs::DEL, &SolarSystem::slowTime, this);
  }
  void update() {
    Canvas* c = currentTab()->getMainCanvas();
    c->getWin()->setDirty();
    double t = time();
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

// cout << "t=" << t << '\t' << "earth rot=" << earthRotationAngle*t << '\n' <<
// tEarth;
#if 0
    for (auto& b : bodies) {
      b.update(t);
    }
#endif
  }
};

int main(int argc, char* argv[]) {
  try {
    return GLWin::init(new SolarSystem(), 800, 800);
  } catch (const char msg[]) {
    cerr << msg << '\n';
  } catch (Ex& e) {
    cout << e << '\n';
  } catch (exception& e) {
    cout << e.what() << '\n';
  } catch (...) {
    cerr << "unknown exception handled\n";
  }
}

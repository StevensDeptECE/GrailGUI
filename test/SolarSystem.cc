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
  Body* orbits;  // the body around which this one orbits, or nullptr if none
  double r;      // radius in m, for OpenGL you will have to scale, float can't
                 // handle space well
  double orbitalRadius;  // distance of circular orbit
  double orbitalFreq;   // inverse of orbital Period, used for relative speed of
                        // animation
  double rotationFreq;  // inverse of rotation Period
  double axialTilt;     // TODO: this should be a full vector, and if you want
                        // accuracy, precession...
  double phase;
  Transformation* trans;

 public:
  Body(Canvas* c, const Style* s, Camera* cam, const std::string& name,
       const char textureFile[], double r, double orbitalRadius,
       double orbitalPeriod, double rotationalPeriod, double axialTilt,
       double startTime, Body* orbits = nullptr);
  void update(double time);
};

Body::Body(Canvas* c, const Style* s, Camera* cam, const std::string& name,
           const char textureFile[], double r, double orbitalRadius,
           double orbitalPeriod, double rotationalPeriod, double axialTilt,
           double startTime, Body* orbits)
    : name(name),
      r(r),
      orbitalRadius(orbitalRadius),
      orbitalFreq(1.0 / orbitalPeriod),
      rotationFreq(1.0 / rotationalPeriod),
      axialTilt(axialTilt * DEG2RAD<double>),
      orbits(orbits) {
  trans = new Transformation();
  MultiShape3D* body =
      c->addLayer(new MultiShape3D(c, cam, textureFile, trans));
  body->genOBJModel("models/sphere.obj");
  update(startTime);
}

void Body::update(double t) {
  trans->ident();  // set to the identity transformation
  // cout << "name= " << name << " orbitalFreq=" << orbitalFreq * t << '\n';
  if (orbits != nullptr) {
    trans->rotate(orbits->orbitalFreq * t, 0, 1, 0);
    trans->translate(orbits->orbitalRadius, 0, 0);
    trans->rotate(-orbits->orbitalFreq * t, 0, 1, 0);
  }

  trans->rotate(orbitalFreq * t, 0, 1, 0);
  trans->translate(orbitalRadius, 0, 0);
  trans->rotate(-orbitalFreq * t, 0, 1, 0);
  trans->rotate(axialTilt, 0.0f, 0.0f, 1.0f);  // TODO: fix general axial tilt
  trans->rotate((rotationFreq * t), 0.0f, 1.0f, 0.0f);
  trans->scale(r);  // scale to the relative size of this body

  // cout << *trans << '\n';
}

// TODO; Implement polar coordinates???
// TODO: Implement panUp/Down for 2D-like views
// TODO: Follow planets

class SolarSystem : public Member {
 private:
  Camera* cam;
  Transformation tSky;
  DynArray<Body*> bodies;  // list of all bodies to be drawn

  constexpr static double YEAR =
      365.2425;  // number of 24-hour days it takes earth to go around the sun
  constexpr static double SIDEREAL_DAY =
      0.9972;  // number of 24-hour "days" it takes earth to rotate once
 public:
  SolarSystem(Tab* tab) : Member(tab, 0, .0125), bodies(10) {
    cam = c->setLookAtProjection(2, 3, 40, 0, 0, 0, 0, 0, 1);
    GLWin* w = tab->getParentWin();
    const Style* s = w->getDefaultStyle();
    const Font* font = w->getDefaultFont();
    defineBindings();
    MultiShape3D* sky = c->addLayer(new MultiShape3D(
        c, cam, "textures/sky1.png", &tSky, "models/spacesky.obj"));
    // tSky.translate(0,0,-10);
    // tSky.scale(10);

    double startTime = tab->time();
    Body* earth;

    bodies.add(earth = new Body(c, s, cam, "Earth", "textures/earth.jpg", 0.4,
                                8, YEAR, SIDEREAL_DAY, 23.5, startTime));
    constexpr double LUNAR_MONTH = 28.5;
    bodies.add(new Body(c, s, cam, "Moon", "textures/moon.jpg", 0.15, 2,
                        LUNAR_MONTH, LUNAR_MONTH, 6, startTime, earth));
    constexpr double MARS_YEAR = 687;
    constexpr double MARS_DAY = 1.14;  // scaled in earth days

    bodies.add(new Body(c, s, cam, "Mars", "textures/mars.jpg", 0.2, 11,
                        MARS_YEAR, MARS_DAY, 0, startTime));
    constexpr double JUPITER_YEAR = 12 * YEAR;
    constexpr double JUPITER_DAY = 0.48;  // scaled in earth days
    Body* jupiter;
    bodies.add(jupiter =
                   new Body(c, s, cam, "Jupiter", "textures/jupiter.jpg", 1.5,
                            14, JUPITER_YEAR, JUPITER_DAY, 0, startTime));

    // TODO: Add some moons of Jupiter
    constexpr double SOLAR_ROTATION =
        30;  // takes about 30 days for the sun to rotate?
    bodies.add(new Body(c, s, cam, "Sun", "textures/sun.jpg", 2, 0, 1,
                        SOLAR_ROTATION, 0, startTime));

    update();
  }

  void panBack();
  void panForward();
  void panRight();
  void panLeft();

  void update() {
    double t = tab->time();
    for (int i = 0; i < bodies.size(); i++) bodies[i]->update(t);
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
  tab->bindEvent(Tab::Inputs::INSERT, &Tab::speedTime, tab);
  tab->bindEvent(Tab::Inputs::DEL, &Tab::slowTime, tab);
  tab->bindEvent(Tab::Inputs::HOME, &Tab::gotoStartTime, tab);
}

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Solar System");
  new SolarSystem(defaultTab);
}

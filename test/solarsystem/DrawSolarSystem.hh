#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "GravSim.hh"
#include "opengl/GrailGUI.hh"

class DrawSolarSystem : public GLWin {
 private:
  // GravSolarSystem s;
  SolarSystemData* s;
  bool nameFlag;

 public:
  DrawSolarSystem(SolarSystemData* s)
      : s(s), nameFlag(true), GLWin(0x000000, 0xCCCCCC, "Test Solar") {}

  void init() { update(); }
  void update() {
    MainCanvas* c = currentTab()->getMainCanvas();
    StyledMultiShape2D* gui = c->getGui();
    MultiText* m = c->getGuiText();
    gui->clear();
    m->clear();
    double ScaleRadius = 2000;
    double scalePosition = 300;
    // double sunRadius = 1392700 / 2;
    for (auto b : s->bodies) {
      // if (b->getName() == "Sun") {
      double r = b->getDiameter() / ScaleRadius;
      if (r > 50) r *= 0.1;
      // cout << b->getPosition().x << endl;
      float screenX = b->getPosition().x * scalePosition + 500;
      float screenY = -b->getPosition().y * scalePosition + 500;
      gui->drawCircle(screenX, screenY, r, 1, grail::purple);
      if (nameFlag) {
        const std::string& name = b->getName();
        m->add(screenX, screenY, name.c_str(), name.length());
      }

      // double newX = ((b->getPosition().x * 1000) / 55);
      // double newY = ((b->getPosition().y * 1000) / 40);
      // double newRadius = (b->getDiameter() / sunRadius) * sunScaleRadius;
      // cout << b->getDiameter() << endl;

      // gui->drawCircle(newX + 500, -newY + 500, newRadius, 1, grail::black);
    }
    gui->update();
    s->advanceTime(1);
    std::cout << *s << std::endl;
  }
};

#include "opengl/GrailGUI.hh"

using namespace std;

// BUG: If Frame Rate exceeds refresh rate (noticed at 120fps where
// default is 60fps), animation breaks at the Tab level
Member::Member(Tab* tab, double frameRate, double dt)
    : tab(tab), c(tab->getMainCanvas()) {
  tab->setFrameRate(frameRate);
  tab->setDt(dt);
  tab->setDefaultDt(0.0001);
  tab->addMember(this);
}

void Member::setFrameRate(double frameRate) { tab->setFrameRate(frameRate); }

void Member::setModelDt(double dt, double defaultDt) {
  tab->setDt(dt);
  tab->setDefaultDt(defaultDt);
}

void Member::init() {}
void Member::render() {}
void Member::update() {}

Tab* Member::getParentTab() { return tab; }
MainCanvas* Member::getParentCanvas() { return c; }
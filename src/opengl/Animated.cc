#include "opengl/Animated.hh"

#include "opengl/GrailGUI.hh"

// BUG: If Frame Rate exceeds refresh rate (noticed at 120fps where
// default is 60fps), animation breaks at the Tab level
Animated::Animated(Tab* tab) : tab(tab), c(tab->getMainCanvas()) {
  tab->setFrameRate(60);
  tab->setDt(0.0001);
}

void Animated::init() {}
void Animated::render() {}
void Animated::update() {}

Tab* Animated::getParentTab() { return tab; }
MainCanvas* Animated::getParentCanvas() { return c; }
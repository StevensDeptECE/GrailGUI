#include "opengl/Animated.hh"

#include "opengl/GrailGUI.hh"

// BUG: If Frame Rate exceeds refresh rate (noticed at 120fps where
// default is 60fps), animation breaks at the Tab level
Animated::Animated(Tab* tab, double frameRate, double dt) : tab(tab), c(tab->getMainCanvas()) {
  //TODO: set hardware refresh rate in OpenGL or GLFW?
  //TODO: if (frameRate > maxFrameRate) { }
  tab->setFrameRate(frameRate);
  tab->setDt(dt);
  tab->setDefaultDt(dt);
}

void Animated::init() {}
void Animated::render() {}
void Animated::update() {}

Tab* Animated::getParentTab() { return tab; }
MainCanvas* Animated::getParentCanvas() { return c; }
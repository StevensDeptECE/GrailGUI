#include "opengl/Animated.hh"

#include "opengl/GrailGUI.hh"
#include <string>

using namespace std;

// BUG: If Frame Rate exceeds refresh rate (noticed at 120fps where
// default is 60fps), animation breaks at the Tab level
Animated::Animated(Tab* tab, double frameRate, double dt) : tab(tab), c(tab->getMainCanvas()) {
  //TODO: set hardware refresh rate in OpenGL or GLFW?
  //TODO: if (frameRate > maxFrameRate) { }
  tab->setFrameRate(frameRate);
  tab->setDt(dt);
  tab->setDefaultDt(dt);
	tab->addAnimated(this);
}

Animated::Animated(Tab* tab, const string& title, double frameRate, double dt)
	: Animated(tab, frameRate, dt) {
	GLWin* w = tab->getParentWin();
	w->setTitle(title);
}

Animated::Animated(Tab* tab, const string& title, uint32_t width, uint32_t height, double frameRate, double dt)
	: Animated(tab, frameRate, dt) {
	GLWin* w = tab->getParentWin();
	w->setTitle(title);
	w->setSize(width,height);
}

void Animated::init() {}
void Animated::render() {}
void Animated::update() {}

Tab* Animated::getParentTab() { return tab; }
MainCanvas* Animated::getParentCanvas() { return c; }

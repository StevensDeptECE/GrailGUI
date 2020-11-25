#include "opengl/Tab.hh"
#include "opengl/Style.hh"
#include "opengl/GLWin.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <iostream>
#include <cstring>
//#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

Tab::Tab(GLWin *parent) : parent(parent), canvases(16), styles(16) {
  const Style* s = parent->getDefaultStyle();
	Canvas* main;
	canvases.add
		(main = new Canvas(parent, s,0,0, parent->width, parent->height,
											 parent->width, parent->height));
	const Font* f = parent->getDefaultFont();
	gui = main->addLayer(new StyledMultiShape2D(s));
	mt = main->addLayer(new MultiText(s, f->getHeight()));
	menus = main->addLayer(new StyledMultiShape2D(s));
  menuText = main->addLayer(new MultiText(s, f->getHeight()));
}

void Tab::init() {
	for (int i = 0; i < canvases.size(); ++i) {
    canvases[i]->init();
  }
}

void Tab::update(){
	for (int i = 0; i < canvases.size(); ++i) {
    canvases[i]->update();
  }
}

void Tab::cleanup() {
	for (int i = 0; i < styles.size(); ++i) {
    delete styles[i];
	}
  styles.clear(); // empty the array since the pointers are all dead
  for(int i = 0; i < canvases.size(); ++i) {
 //TODO: just delete and let this call cleanup itself
    delete canvases[i];
  }
  canvases.clear(); // empty the array since the pointers are all dead
}

void Tab::render() {
  for(int i = 0; i < canvases.size(); ++i) {
    canvases[i]->render();
  }
}

Tab::~Tab() { 
  cleanup();
}

Canvas* Tab::addCanvas(const Style* style,
											 uint32_t vpX, uint32_t vpY, uint32_t vpW, uint32_t vpH) {
	Canvas* c = new Canvas(parent, style, vpX,vpY,vpW,vpH,vpW,vpH);
	canvases.add(c);
	return c;
}

void Tab::addButton(const Style* s, const char text[],
										float x, float y, float w, float h) {
	gui->fillRectangle(x, y, w, h, s->bg);
	mt->add(x, y, s->f, text, strlen(text));
}

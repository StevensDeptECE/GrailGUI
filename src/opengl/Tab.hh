#pragma once

#include <cstdint>
#include "opengl/Canvas.hh"
#include "util/DynArray.hh"
/*
TODO: Add border and other kinds of style info.
This could go into several classes related by inheritance in some way.
We have to decide how much style we want to support. We're not doing css3,
but we could do more than just font and color.  Perhaps borders? Patterns?
Background pictures?

Predefine an array of images so they can be referred at runtime with an int
 */
class GLWin;
class StyledMultiShape2D;
class MultiText;
class Tab {
private:
  DynArray<Canvas*> canvases;
  DynArray<Style*> styles;

	// hardcoded, predefined objects used for GUI
	// For anything custom, you should use your own objects, not use these
	StyledMultiShape2D* gui;
  MultiText *mt;
	StyledMultiShape2D* menus; // special layer for menus and anything else in front of page
	MultiText* menuText;
  GLWin* parent;
public:
  Tab(GLWin* parent);
  ~Tab();
  GLWin* getParent() const { return parent; }

  //copying a tab is not supported. What would it mean?
  Tab(const Tab& orig) = delete;
  Tab& operator =(const Tab& orig) = delete;
  Canvas* addCanvas
	(const Style* style, uint32_t vpX, uint32_t vpY, uint32_t vpW, uint32_t vpH);

  Canvas* getCanvas(uint32_t i){
    return canvases[i];
  }

  Canvas* getMainCanvas(){
    return canvases[0];
  }

  void init();
  void update();
  void render();
  void cleanup();
	void addButton(const Style* s, const char text[],
								 float x, float y, float w, float h);
	void addMenu(float x, float y);
	template<typename Widget>
	void add(Widget w, float x, float y);
};

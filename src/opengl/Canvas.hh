#pragma once

//#include <cstring>

#include <set>

#include "opengl/GLWin.hh"
#include "opengl/Shape.hh"
#include "util/DynArray.hh"

class Camera;
class Style;
class Tab;
class Canvas {
 protected:
  GLWin* w;
  Tab* tab;
  DynArray<Shape*> layers;
  uint32_t vpX, vpY, vpW, vpH;  // viewport
  uint32_t pX, pY;              // projection
  glm::mat4 projection;         // the projection currently used
  glm::mat4
      originalProjection;  // keep safe so you can reset to the original view
  const Style* style;
  Camera* cam;

 public:
  Canvas(Tab* tab);
  Canvas(GLWin* w, Tab* tab)
      : Canvas(w, tab, w->getDefaultStyle(), 0, 0, w->getWidth(),
               w->getHeight(), w->getWidth(), w->getHeight()) {}

  Canvas(GLWin* w, Tab* tab, const Style* style, uint32_t vpX, uint32_t vpY,
         uint32_t vpW, uint32_t vpH, uint32_t pX,
         uint32_t pY)
      :  // viewport, projection
        w(w),
        tab(tab),
        layers(4),
        style(style),
        vpX(vpX),
        vpY(vpY),
        vpW(vpW),
        vpH(vpH),
        pX(pX),
        pY(pY),
        cam(nullptr) {
    projection =
        glm::ortho(0.0f, static_cast<float>(pX), static_cast<float>(pY), 0.0f);
    originalProjection = projection;
    //    projection = glm::scale(projection, glm::vec3(16, -16, 1));
    //    projection = glm::translate(projection, glm::vec3(180, -90, 0));
  }
  ~Canvas();
  Canvas(const Canvas& orig) = delete;
  Canvas& operator=(const Canvas& orig) = delete;
  uint32_t getWidth() const { return vpW; }
  uint32_t getHeight() const { return vpH; }
  GLWin* getWin() const { return w; };
  Tab* getTab() const { return tab; }
  glm::mat4* getProjection() { return &projection; }
  void setProjection(const glm::mat4& proj) { projection = proj; }

  void setOrthoProjection(float xLeft, float xRight, float yBottom,
                          float yTop) {
    projection = glm::ortho(xLeft, xRight, yBottom, yTop);
  }
  Camera* setLookAtProjection(float eyeX, float eyeY, float eyeZ, float lookAtX,
                              float lookAtY, float lookAtZ, float upX,
                              float upY, float upZ);
  void resetProjection() { projection = originalProjection; }

  // Add layer pointer and return its index
  template <typename S>
  S* addLayer(S* shape) {
    layers.add(shape);
    return shape;
  }

  Shape* getLayer(uint32_t i) {
    if (i < layers.size()) return layers[i];
    return nullptr;
  }

  void init() {
    for (int i = 0; i < layers.size(); i++) {
      layers[i]->init();
    }
  }

  void process_input(Inputs* in, float dt) {
    for (int i = 0; i < layers.size(); i++) {
      layers[i]->process_input(in, dt);
    }
  }

  void update() {
    for (int i = 0; i < layers.size(); i++) {
      layers[i]->update();
    }
  }

  const Style* getStyle() const { return style; }

  void render();

  void cleanup();
};

class StyledMultiShape2D;
class MultiText;
class InteractiveWidget2D;
class MainCanvas : public Canvas {
 private:
  StyledMultiShape2D* gui;
  MultiText* guiText;
  StyledMultiShape2D* menu;
  MultiText* menuText;
  std::set<InteractiveWidget2D*> widgets;

 public:
  MainCanvas(Tab* tab);
  ~MainCanvas();
  MainCanvas(const MainCanvas&) = delete;
  MainCanvas& operator=(const MainCanvas&) = delete;
  StyledMultiShape2D* getGui() { return gui; }
  MultiText* getGuiText() { return guiText; }
  StyledMultiShape2D* getMenu() { return menu; }
  MultiText* getMenuText() { return menuText; }

  void init();
  void render();
  void cleanup();

  void click();

  void addClickableWidget(InteractiveWidget2D* w) { widgets.insert(w); }
  void addButton(const char text[], float x, float y, float w, float h);
  void addLabel(const char text[], float x, float y, float w, float h);
  void addMenu(const std::string menu[], uint32_t numStrings, float x, float y);
};

#pragma once

#include <cstdint>

#include "opengl/Canvas.hh"
#include "util/Callbacks.hh"
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
class Member;
class Tab : public CallbackHandler {
 private:
  DynArray<Member*> members;
  DynArray<Canvas*> canvases;
  DynArray<Style*> styles;

 public:
  double startTime;  // physical start time for simulations (zero by default)
  double t;          // master time variable for animations
  double dt;         // delta time advanced every frame
  double defaultDt;  // original delta time for each frame
  double endTime;    // end time for simulations,

  double lastUpdateTime;  // last time this tab drew a new animation
  // TODO: every time you switch tabs, reset this number so user gets to see
  // animation for full time period without skipping
  double updateTime;  // how much time for each frame
  GLWin* parent;
  MainCanvas mainCanvas;  // special canvas for drawing GUI and menus
  // For anything custom, you should use your own objects, not use these

 public:
  Tab(GLWin* parent);
  ~Tab();

  /**
   * @brief Return model (physical) time for a program that needs a single
   * timebase
   *
   * @return float
   */
  double time() const { return t; }
  void setTime(float t);     // set t to any desired value
  void resetAnim();          // set t=0
  void tick();               // move t forward
  double getTime() const;    // Get GLFW's time
  void setEndTime(float t);  // define the end time. When end is reached restart

  /**
   * @brief set Delta time, the amount time advances each animation step
   *
   */
  void setDt(double delta) { dt = delta; }

  void setDefaultDt(double delta) { defaultDt = delta; }

  void setFrameRate(double framerate) {
    updateTime = (framerate == 0) ? 0 : (1.0 / framerate);
  }

  /**
   * @brief return true if this tab needs to be updated (redrawn)
   *
   */
  bool checkUpdate();

  GLWin* getParentWin() const { return parent; }

  // copying a tab is not supported. What would it mean?
  Tab(const Tab& orig) = delete;
  Tab& operator=(const Tab& orig) = delete;
  Canvas* addCanvas(const Style* style, uint32_t vpX, uint32_t vpY,
                    uint32_t vpW, uint32_t vpH);

  Canvas* getCanvas(uint32_t i) { return canvases[i]; }

  void addMember(Member* mem) { members.add(mem); }

  MainCanvas* getMainCanvas() { return &mainCanvas; }

  const Font* getDefaultFont() const { return parent->getDefaultFont(); }
  const Font* getGuiFont() const { return parent->getGuiFont(); }
  const Font* getMenuFont() const { return parent->getMenuFont(); }

  void setRender() const { parent->setRender(); }
  void setUpdate() const { parent->setUpdate(); }

  void loadBindings();

  void init();
  void update();
  void render();
  void cleanup();

  void gotoStartTime();
  void gotoEndTime();
  void speedTime();
  void slowTime();
  void resetTimeDilation();

  void resetProjection2D();
  void zoomIn2D();
  void zoomOut2D();
  void panRight2D();
  void panLeft2D();
  void panUp2D();
  void panDown2D();
  /*
    Page environment (like a book reader)
  */

  void gotoTop();
  void gotoBottom();
  void scrollUp();
  void scrollDown();
  void pageUp();
  void pageDown();
  void sectionUp();
  void sectionDown();

  void clearSelected();

  void resetProjection3D();
  void zoomOut3D();
  void zoomIn3D();
  void panRight3D();
  void panLeft3D();
  void panUp3D();
  void panDown3D();
  void selectObject3D();
  void addSelectObject3D();
  void toggleSelectObject3D();
};

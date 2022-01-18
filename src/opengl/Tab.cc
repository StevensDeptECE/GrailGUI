#include "opengl/Tab.hh"

#include <cstring>
#include <iostream>

#include "opengl/GLWin.hh"
#include "opengl/Member.hh"
#include "opengl/MultiText.hh"
#include "opengl/Style.hh"
#include "opengl/StyledMultiShape2D.hh"
//#include <time.h>

//#ifdef _WIN32
//#include <windows.h>
//#endif
using namespace std;

Tab::Tab(GLWin *parent)
    : CallbackHandler(),
      startTime(0),
      t(startTime),
      dt(1),
      endTime(0),
      parent(parent),
      canvases(16),
      styles(16),
      members(4),
      mainCanvas(this) {}

void Tab::init() {
  for (int i = 0; i < members.size(); ++i) {
    members[i]->init();
  }
  for (int i = 0; i < canvases.size(); ++i) {
    canvases[i]->init();
  }
  mainCanvas.init();
}

/**
 * Frames can set animation rates. If set to -1, never animates (static)
 * 0 means draw as fast as possible, no delay (set needsUpdate=true which will
 * force update()) positive number waits this much time. So for two second delay
 * should be 2.0 This is setusing setFrameRate() which sets the inverse, ie
 * setFrameRate(60) for 60 fps.
 *
 * @return true if update is needed
 */
// BUG: If framerate is set to 0 when instantiating a Member, then the resulting
// animation is much slower than when framerate is set to 60. This may be due to
// Member, Tab, GLWin, or something else, so the bug is getting put here for
// now.
bool Tab::checkUpdate() {
  if (updateTime == 0 ||
      (updateTime > 0 && glfwGetTime() > lastUpdateTime + updateTime)) {
    lastUpdateTime = glfwGetTime();
    return true;
  }
  return false;
}

void Tab::update() {
  for (int i = 0; i < members.size(); ++i) {
    members[i]->update();
  }

  for (int i = 0; i < canvases.size(); ++i) {
    canvases[i]->update();
  }

  mainCanvas.update();
}

void Tab::cleanup() {
  for (int i = 0; i < styles.size(); ++i) {
    delete styles[i];
  }
  styles.clear();  // empty the array since the pointers are all dead
  for (int i = 0; i < canvases.size(); ++i) {
    // TODO: just delete and let this call cleanup itself
    delete canvases[i];
  }
  canvases.clear();  // empty the array since the pointers are all dead
                     // TODO: mainCanvas.cleanup()?
}

void Tab::render() {
  for (int i = 0; i < members.size(); ++i) {
    members[i]->render();
  }

  for (int i = 0; i < canvases.size(); ++i) {
    canvases[i]->render();
  }

  mainCanvas.render();
}

Tab::~Tab() { cleanup(); }

Canvas *Tab::addCanvas(const Style *style, uint32_t vpX, uint32_t vpY,
                       uint32_t vpW, uint32_t vpH) {
  Canvas *c = new Canvas(parent, this, style, vpX, vpY, vpW, vpH, vpW, vpH);
  canvases.add(c);
  return c;
}

/*
  Actions pertaining to the time axis.
  This should work independently of any projection.
  If you don't want time in your application, just ignore this.
  However, the minimal amount of code provides very useful
  API for anyone interested in moving through data in time.

  TODO: We could add an action to go to a particular point in time but
  we don't currently have a way to pass parameters to actions
  This is going to change.
*/
void Tab::gotoStartTime() { t = startTime; }
void Tab::gotoEndTime() { t = endTime; }
void Tab::speedTime() { dt *= 2; }
void Tab::slowTime() { dt *= 0.5; }
void Tab::resetTimeDilation() { dt = defaultDt; }

void Tab::tick() { t += dt; }

double Tab::getTime() const { return glfwGetTime(); }

/*
  actions for a 2d environment
*/

void Tab::resetProjection2D() { mainCanvas.resetProjection(); }

void Tab::zoomIn2D() {
  glm::mat4 *proj = mainCanvas.getProjection();
  *proj = glm::scale(*proj,
                     glm::vec3(1.2));  // TODO: make zoom in factor a variable
}

void Tab::zoomOut2D() {
  glm::mat4 *proj = mainCanvas.getProjection();
  *proj = glm::scale(*proj, glm::vec3(1 / 1.2));
}

void Tab::panRight2D() {
  glm::mat4 *proj = mainCanvas.getProjection();
  const float x = -100;  // TODO: make this in model coordinates!
  *proj = glm::translate(*proj, glm::vec3(x, 0, 0));
}

void Tab::panLeft2D() {
  glm::mat4 *proj = mainCanvas.getProjection();
  const float x = +100;  // TODO: make this in model coordinates!
  *proj = glm::translate(*proj, glm::vec3(x, 0, 0));
}

void Tab::panUp2D() {
  glm::mat4 *proj = mainCanvas.getProjection();
  const float y = 100;  // TODO: make this in model coordinates!
  *proj = glm::translate(*proj, glm::vec3(0, y, 0));
}

void Tab::panDown2D() {
  glm::mat4 *proj = mainCanvas.getProjection();
  const float y = -100;  // TODO: make this in model coordinates!
  *proj = glm::translate(*proj, glm::vec3(0, y, 0));
}

void Tab::gotoTop() {}
void Tab::gotoBottom() {}
void Tab::scrollUp() {}
void Tab::scrollDown() {}
void Tab::pageUp() {}
void Tab::pageDown() {}
void Tab::sectionUp() {}
void Tab::sectionDown() {}
void Tab::zoomOut3D() {}
void Tab::zoomIn3D() {}
void Tab::panRight3D() {}
void Tab::panLeft3D() {}
void Tab::panUp3D() {}
void Tab::panDown3D() {}
void Tab::selectObject3D() {}
void Tab::addSelectObject3D() {}
void Tab::toggleSelectObject3D() {}
void Tab::resetProjection3D() {}

void Tab::loadBindings() {
  using namespace std::placeholders;
  using CBSec = CallbackHandler::Security;

  registerAction(CBSec::RESTRICTED, &GLWin::quit, parent);
  registerAction(CBSec::SAFE, &GLWin::refresh, parent);
  registerAction(CBSec::ASK, &GLWin::saveFrame, parent);

  registerAction(CBSec::SAFE, &Tab::gotoStartTime, this);
  registerAction(CBSec::SAFE, &Tab::gotoEndTime, this);
  registerAction(CBSec::SAFE, &Tab::speedTime, this);
  registerAction(CBSec::SAFE, &Tab::slowTime, this);
  registerAction(CBSec::SAFE, &Tab::resetTimeDilation, this);

  registerAction(CBSec::SAFE, &Tab::resetProjection3D, this);
  registerAction(CBSec::SAFE, &Tab::zoomOut3D, this);
  registerAction(CBSec::SAFE, &Tab::zoomIn3D, this);
  registerAction(CBSec::SAFE, &Tab::panRight3D, this);
  registerAction(CBSec::SAFE, &Tab::panLeft3D, this);
  registerAction(CBSec::SAFE, &Tab::panUp3D, this);
  registerAction(CBSec::SAFE, &Tab::panDown3D, this);
  registerAction(CBSec::SAFE, &Tab::selectObject3D, this);
  registerAction(CBSec::SAFE, &Tab::addSelectObject3D, this);
  registerAction(CBSec::SAFE, &Tab::toggleSelectObject3D, this);

  registerAction(CBSec::SAFE, &Tab::resetProjection2D, this);
  registerAction(CBSec::SAFE, &Tab::zoomOut2D, this);
  registerAction(CBSec::SAFE, &Tab::zoomIn2D, this);
  registerAction(CBSec::SAFE, &Tab::panRight2D, this);
  registerAction(CBSec::SAFE, &Tab::panLeft2D, this);
  registerAction(CBSec::SAFE, &Tab::panUp2D, this);
  registerAction(CBSec::SAFE, &Tab::panDown2D, this);

  registerAction(CBSec::SAFE, &Tab::gotoTop, this);
  registerAction(CBSec::SAFE, &Tab::gotoBottom, this);
  registerAction(CBSec::SAFE, &Tab::scrollUp, this);
  registerAction(CBSec::SAFE, &Tab::scrollDown, this);
  registerAction(CBSec::SAFE, &Tab::pageUp, this);
  registerAction(CBSec::SAFE, &Tab::pageDown, this);
  registerAction(CBSec::SAFE, &Tab::sectionUp, this);
  registerAction(CBSec::SAFE, &Tab::sectionDown, this);

  // TODO: How to define actions that take parameters, in this case a string?
  //  registerAction(CBSec::SAFE, playSound);

  // bind3D();
  // bind2DOrtho();
}
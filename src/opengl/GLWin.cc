#if _MSC_VER >= 1600 /* 1600 is Microsoft Visual Studio 2010 */
#pragma execution_character_set("utf-8")
#endif

#include "GLWin.hh"

#include <unistd.h>

#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "opengl/Errcode.hh"
#include "util/Ex.hh"

// glad seems "unhappy" if you include it after glfw. Why?
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include "csp/Socket.hh"
#include "util/Prefs.hh"

#ifdef _WIN32
#include <windows.h>
#endif
#include <cstring>

#include "opengl/Style.hh"
#include "opengl/Tab.hh"
#include "stb/stb_image_write.h"
#include "xdl/std.hh"
using namespace std;
string GLWin::baseDir;

unordered_map<GLFWwindow *, GLWin *> GLWin::winMap;
default_random_engine gen;
uniform_real_distribution<double> u01(0.0, 1.0);
glm::mat4 GLWin::projection;

uint32_t GLWin::inputMap[32768];
Action GLWin::actionMap[4096];

/* GLFW Callbacks */

void GLWin::cursorPositionCallback(GLFWwindow *win, double xpos, double ypos) {
  GLWin *w = GLWin::getWin(win);
  if (w == nullptr) {
    cerr << "shouldn't be possible, but window is null! Uggh!\n";
    return;
  }
  glfwGetCursorPos(win, &w->mouseX, &w->mouseY);
  if (w->dragMode) {
    cerr << (xpos - w->mousePressX) << " " << (ypos - w->mousePressY) << '\n';
  }
}

void GLWin::cursorEnterCallback(GLFWwindow *win, int entered) {
  GLWin *w = GLWin::getWin(win);
  if (entered) {
    /* Update mouse position and
     * disable X11 cursor */
    glfwGetCursorPos(win, &w->mouseX, &w->mouseY);
    // GLWin::mouse_leaved = 0;
    // GLWin::mouse_entered = 1;
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    /* Mouse has left window
     * Enable X11 cursor */
    /* Not sure if needed anymore */
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    // GLWin::mouse_leaved = 1;
  }
}

void GLWin::windowPositionCallback(GLFWwindow *win, int xpos, int ypos) {
  GLWin *w = GLWin::getWin(win);
  w->winXPos = xpos, w->winYPos = ypos;
}

void GLWin::windowFocusCallback(GLFWwindow *win, int focused) {
  GLWin *w = GLWin::getWin(win);
  w->focused = focused != 0;
  if (focused) {
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }
}

inline void GLWin::doit(GLWin *w, uint32_t input) {
  if (w == nullptr) {
    cerr << "no mapping for window to GLWin\n";
    return;  // TODO: come up with better response? Should never
  }          // happen
  uint32_t act = GLWin::inputMap[input];
  if (act == 0) return;
  Action a = GLWin::actionMap[act];
  a(w);  // execute the action
}

void GLWin::keyCallback(GLFWwindow *win, int key, int scancode, int action,
                        int mods) {
  uint32_t input = (mods << 9) | key;
  cerr << "key: " << key << " mods: " << mods << " input=" << input << '\n';
  doit(winMap[win], input);
}

void GLWin::mouseButtonCallback(GLFWwindow *win, int button, int action,
                                int mods) {
  // find the button and click on it.
  //  ((Grail*)this) ->currentTab()->processMouseEvent
  // bool consumed = gui.clickOnObject(button, (uint32_t)mouseXPos,
  // (uint32_t)mouseYPos);
  GLWin *w = winMap[win];
  uint32_t input = (mods << 9) | (action << 3) | button;
  cerr << "mouse! " << button << ", action=" << action << w->mouseX << ","
       << w->mouseY << " input=" << input << '\n';
  doit(w, input);
}

void GLWin::scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
  cout << "xoffset=" << xoffset << " yoffset=" << yoffset << '\n';
  // todo: we would have to copy offsets into the object given the way this is
  uint32_t input = 400;
  doit(winMap[win], input + int(yoffset));
}

void GLWin::windowRefreshCallback(GLFWwindow *win) {
  GLWin *w = GLWin::getWin(win);
  w->dirty = true;
}

#if 0
void GLWin::enableMouse() {
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
#endif

// Static initializer for libraries
void GLWin::classInit() {
  XDLType::classInit();
  Socket::classInit();
}

// Static cleanup for libraries
void GLWin::classCleanup() {
  Socket::classCleanup();
  XDLType::classCleanup();
}

GLWin::GLWin(uint32_t bgColor, uint32_t fgColor, const char title[],
             uint32_t exitAfter)
    : bgColor(bgColor),
      fgColor(fgColor),
      exitAfter(exitAfter),
      startTime(0),
      endTime(0),
      t(startTime),
      dt(1),
      tabs(4),
      faces(16) {
  if (title != nullptr) {
    this->title = title;
  } else {
    this->title = "";
  }
  for (int i = 0; i < 3; i++) numActions[i] = 0;
  loadBindings();
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE);  // uncomment this statement to fix compilation on OS X
#endif
  // all static library initializations go here
  XDLType::classInit();
  Socket::classInit();
}
bool GLWin::hasBeenInitialized = false;
GLWin::GLWin(uint32_t w, uint32_t h, uint32_t bgColor, uint32_t fgColor,
             const char title[], uint32_t exitAfter)
    : GLWin(bgColor, fgColor, title, exitAfter) {
  setSize(w, h);
  startWindow();
}

void GLWin::startWindow() {
  win = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (win == nullptr) {
    glfwTerminate();
    throw "Failed to open GLFW window";
  }
  winMap[win] = this;
  //  cerr << winMap[win] << '\n';
  //  winMap[win] = this;
  glfwMakeContextCurrent(win);
  glfwSetWindowSizeCallback(win, resize);
  //	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw "Failed to initialize GLAD";
  }
  glfwSetCursorPosCallback(win, GLWin::cursorPositionCallback);
  glfwSetMouseButtonCallback(win, GLWin::mouseButtonCallback);
  glfwSetKeyCallback(win, keyCallback);
  glfwSetScrollCallback(win, GLWin::scrollCallback);
  glfwSetWindowRefreshCallback(win, GLWin::windowRefreshCallback);

  // it seems like glfw will not support good mouse behavior unless we hide the
  // cursor? ugly.
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  // glEnable(GL_CULL_FACE); I disable this because when we
  // change the projection to be normal screen pixels than it doesnt draw since
  // its drawing it in the opposite orientation i assume
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_TEXTURE);
  glDepthFunc(GL_NEVER);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLWin::projection = glm::ortho(0.0f, static_cast<GLfloat>(width),
                                 static_cast<GLfloat>(height), 0.0f);
  // std::cerr << width << " " << height << std::endl;
  // this test is designed to make sure that multiple windows will only
  // initialize fonts once baseDir is not as serious, but why not do any
  // singleton initialization here
  // TODO: is there any more elegant way?
  if (!hasBeenInitialized) {
    *(string *)&baseDir = getenv("GRAIL");
    FontFace::initAll();
  }
  defaultFont = (Font *)FontFace::get("TIMES", 16, FontFace::BOLD);
  Font *bigFont = (Font *)FontFace::get("TIMES", 20, FontFace::BOLD);
  Font *normalFont = (Font *)FontFace::get("TIMES", 12, FontFace::NORMAL);
  guiFont = bigFont;
  menuFont = bigFont;

  defaultStyle = new Style(defaultFont, 0, 0, 0, 1, 0, 0, COMMON_SHADER);
  defaultStyle->setLineWidth(1);
  //  defaultStyle->setShaderIndex(COMMON_SHADER);
  guiStyle = new Style(guiFont, 0, 0, 0, 1, 0, 0, COMMON_SHADER);
  guiTextStyle = new Style(guiFont, 0, 0, 0, 1, 0, 0, COMMON_SHADER);
  menuStyle = new Style(menuFont, 0, 0, 0, 1, 0, 0, COMMON_SHADER);
  menuTextStyle = new Style(menuFont, 0, 0, 0, 1, 0, 0, COMMON_SHADER);
  current = new Tab(this);
  tabs.add(current);
  hasBeenInitialized = true;
}
void GLWin::baseInit() {
  Shader::setDir(prefs.getShaderDir());
  Shader::load("solid.bin", "common.vs", "common.fs");    // Solid Color
  Shader::load("pervert.bin", "vColor.vs", "common.fs");  // Color per vertex
  Shader::load("text.bin", "text.vs", "text.fs");         // Texture for text
  Shader::load("img.bin", "Texture.vs", "Texture.fs");    // Texture for images
  Shader::load("cursor.bin", "Cursor.vs", "common.fs");   // Texture for images
  Shader::load("multiText.bin", "MultiTexture.vs",
               "MultiTexture.fs");  // MultiTexture for shapes
  for (int i = 0; i < tabs.size(); ++i) {
    tabs[i]->init();
  }
}

float getTime() const { return glfwGetTime(); }

int GLWin::init(GLWin *g, uint32_t w, uint32_t h, uint32_t exitAfter) {
  try {
    g->exitAfter = exitAfter;
    g->setSize(w, h);
    g->startWindow();
    // g->t = thread(crun, g);
    g->mainLoop();
    FontFace::emptyFaces();
    delete g;  // free up the memory given to us (must use new!)
  } catch (const Ex &e) {
    cerr << e << '\n';
  } catch (const char *msg) {
    cerr << msg << endl;
  } catch (const std::exception &e) {
    cerr << e.what() << endl;
  } catch (...) {
    cerr << "uncaught exception! (ouch)\n";
  }
  return 0;
}

void GLWin::cleanup() {
  for (int i = 0; i < tabs.size(); ++i) {
    delete tabs[i];  // TODO:cw[i]->cleanup();
  }
  tabs.clear();
  delete defaultStyle;
  defaultStyle = nullptr;
  Shader::cleanAll();
}

void GLWin::init() {}
void GLWin::render() {
  for (int i = 0; i < tabs.size(); ++i) {
    tabs[i]->render();
  }
}
// default is no animation. Override if you want your class to animate
void GLWin::update() {}

// declare prototype access to static function in other file which calls
// FaceFont::cleanup()
void FontFaceCleanup();

GLWin::~GLWin() {
  cleanup();
  cerr << "GLWin Destructor" << endl;
  Socket::classCleanup();
  XDLType::classCleanup();
}

// void GLWin::addFontPath(std::string path, std::string name) {
//   name.erase(name.end() - 4, name.end());

//   pathByName[name] = path;
//   // std::cout<<"Added: " << name.c_str()<<std::endl;
// }

void GLWin::mainLoop() {
  init();      // call the child class method to set up
  baseInit();  // call grails initialization for shaders

  float lastFrame = 0;

  const double frameLimit = 1 / 60.0;
  double lastRender = 0;
  uint32_t frameCount = 0;
  double startTime = glfwGetTime();  // get time now for calculating FPS
  double renderTime;
  dirty = true;
  dirty2 = false;
  while (!glfwWindowShouldClose(win)) {
    //    bool modified = Queue::dump_render();
    //    dt = current - lastFrame;
    float startRender = glfwGetTime();
    glfwPollEvents();  // Check and call events

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Clear the colorbuffer and depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();
    glfwSwapBuffers(win);  // Swap buffer so the scene shows on screen
    float endRender = glfwGetTime();
    renderTime += endRender - startRender;
    if (frameCount >= 150) {
      double endTime = glfwGetTime();
      double elapsed = endTime - startTime;
      cerr << "Elapsed=" << elapsed << " FPS= " << frameCount / elapsed
           << " render=" << renderTime << '\n';
      frameCount = 0;
      renderTime = 0;
      startTime = endTime;
      if (exitAfter != 0 && frameCount >= exitAfter) break;
    } else {
      frameCount++;
    }

    dirty = false;
    if (dirty2) {
      dirty = true;
      dirty2 = false;
    }
    t += dt;
    update();
  }
  cleanup();
  glfwDestroyWindow(win);
  glfwTerminate();
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void GLWin::resize(GLFWwindow *win, int width, int height) {
  (0, 0, width, height);
}

void GLWin::random(glm::vec3 &v) {
  v.x = u01(gen), v.y = u01(gen), v.y = u01(gen);
}

void GLWin::quit(GLWin *w) {
  exit(0);  // TODO: check for cleanup first?
}

void GLWin::refresh(GLWin *w) { w->setDirty(); }

void GLWin::saveFrame(GLWin *w) {
  if (w->saveBuffer != nullptr && w->saveW != w->width ||
      w->saveH != w->height) {
    delete[] w->saveBuffer;
    w->saveBuffer = nullptr;
  }
  if (w->saveBuffer == nullptr) {
    strncpy(w->frameName, "tmp/frame", sizeof(w->frameName));
    w->saveW = w->width, w->saveH = w->height;
    w->saveBuffer = new uint8_t[3 * w->saveW * w->saveH];
  }
  sprintf(w->frameName + 8, "%d.png", w->frameNum++);
  glReadPixels(0, 0, w->saveW, w->saveH, GL_RGB, GL_UNSIGNED_BYTE,
               w->saveBuffer);
  constexpr int CHANNELS = 3;
  stbi_write_png(w->frameName, w->saveW, w->saveH, CHANNELS, w->saveBuffer,
                 w->saveW * CHANNELS);  // TODO: Remove - debugging purposes
}

/*
  Below here are all the predefined functions that are bound to actions.
  These come in bundles. Not all functions apply to the same situations.

  Currently envisioned are functions to handle interactions:
  1. In a 3d environment like solarsystem where there is a master time and a 3d
  camera
  2. A 2d environment like a map viewer. Data can also be viewed using the time
  variable
  3. In a document viewer environment where the user is moving through a 1d or
  2d surface

  Other kinds of interactions to be added as they come up. The idea is to
  provide a generic way to handle these kind of common interaction models so
  individual programmers do not have to start from scratch. Of course, a
  programmer can always ignore these predefined functions and roll their own set
  of actions.

  Actions are currently functions that can manipulate the window state. They
  take only a single parameter (the window) and have no access to the specifics
  of the user's child of GLWin. In general, however, we want to be able to pass
  actions an integer, a float, a string, perhaps a few other possibilities.

  The question is how to do this, because actions are bound to events.
  When a key is pressed, what value can be passed to an action? By providing
  standard answers to this question, programming can be made a lot simpler and a
  lot more pleasant for graphical applications.

  For example, in a CAD package there can be a standard way of entering a
  number, and when the user hits a key or presses the mouse, that number can be
  used in the action. Currently, every application invents their own way of
  doing this. Grail is going to attempt to provide a standard for this, which
  will improve user experience (if many applications use the same methods, users
  are more comfortable) and improve programmer productivity (programmers won't
  have to roll their own).

  There will always be video games or CAD packages that use their own custom
  event handling. That's fine. But by providing a single clean API that handles
  most cases, Grail can reduce complexity for the average case, hopefully
  dramatically reducing the code size. Already we can see the difference. The
  average OpenGL demo that has user interaction is punctuated by lots of if
  statements checking for specific keys.
*/

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
void GLWin::gotoStartTime(GLWin *w) { w->t = w->startTime; }
void GLWin::gotoEndTime(GLWin *w) { w->t = w->endTime; }
void GLWin::speedTime(GLWin *w) { w->dt *= 2; }
void GLWin::slowTime(GLWin *w) { w->dt *= 0.5; }
void GLWin::resetTimeDilation(GLWin *w) { w->dt = 1; }

/*
  actions for selecting objects in a scene
*/
void GLWin::clearSelected(GLWin *w) {
  // TODO: clear the list of selected objects
}

/*
  actions for a 3d environment
*/
void GLWin::resetProjection3D(GLWin *w) {
  // TODO: call resetCamera on every canvas to reset to initial state (we have
  // to write that)
}

// TODO: implement 3d uniform zoom/pan controls
void GLWin::zoomIn3D(GLWin *w) {}
void GLWin::zoomOut3D(GLWin *w) {}
void GLWin::panRight3D(GLWin *w) {}
void GLWin::panLeft3D(GLWin *w) {}
void GLWin::panUp3D(GLWin *w) {}
void GLWin::panDown3D(GLWin *w) {}

void GLWin::selectObject3D(GLWin *w) {
  // TODO: fire a ray at w->mouseX, w->mouseY and find the object selected
  // set the selected list to that object.
}
void GLWin::addSelectObject3D(GLWin *w) {
  // TODO: fire a ray at w->mouseX, w->mouseY and find the object selected
  // add object to the selected list
}
void GLWin::toggleSelectObject3D(GLWin *w) {
  // TODO: fire a ray at w->mouseX, w->mouseY and find the object selected
  // if object is not in  the selected list put it there. if it is there, remove
  // it
}

/*
  actions for a 2d environment
*/

void GLWin::resetProjection2D(GLWin *w) {
  w->currentTab()->getMainCanvas()->resetProjection();
}

void GLWin::zoomIn2D(GLWin *w) {
  MainCanvas *c = w->currentTab()->getMainCanvas();
  glm::mat4 *proj = c->getProjection();
  *proj = glm::scale(*proj,
                     glm::vec3(1.2));  // TODO: make zoom in factor a variable
}

void GLWin::zoomOut2D(GLWin *w) {
  MainCanvas *c = w->currentTab()->getMainCanvas();
  glm::mat4 *proj = c->getProjection();
  *proj = glm::scale(*proj, glm::vec3(1 / 1.2));
}

void GLWin::panRight2D(GLWin *w) {
  MainCanvas *c = w->currentTab()->getMainCanvas();
  glm::mat4 *proj = c->getProjection();
  const float x = -100;  // TODO: make this in model coordinates!
  *proj = glm::translate(*proj, glm::vec3(x, 0, 0));
}

void GLWin::panLeft2D(GLWin *w) {
  MainCanvas *c = w->currentTab()->getMainCanvas();
  glm::mat4 *proj = c->getProjection();
  const float x = +100;  // TODO: make this in model coordinates!
  *proj = glm::translate(*proj, glm::vec3(x, 0, 0));
}

void GLWin::panUp2D(GLWin *w) {
  MainCanvas *c = w->currentTab()->getMainCanvas();
  glm::mat4 *proj = c->getProjection();
  const float y = 100;  // TODO: make this in model coordinates!
  *proj = glm::translate(*proj, glm::vec3(0, y, 0));
}

void GLWin::panDown2D(GLWin *w) {
  MainCanvas *c = w->currentTab()->getMainCanvas();
  glm::mat4 *proj = c->getProjection();
  const float y = -100;  // TODO: make this in model coordinates!
  *proj = glm::translate(*proj, glm::vec3(0, y, 0));
}

void GLWin::pressOnWidget(GLWin *w) {
  w->mousePressX = w->mouseX, w->mousePressY = w->mouseY;
  w->dragMode = true;
}

void GLWin::releaseWidget(GLWin *w) { w->dragMode = false; }
/*
  Page environment (like a book reader)
*/

void GLWin::gotoTop(GLWin *w) {}
void GLWin::gotoBottom(GLWin *w) {}
void GLWin::scrollUp(GLWin *w) {}
void GLWin::scrollDown(GLWin *w) {}
void GLWin::pageUp(GLWin *w) {}
void GLWin::pageDown(GLWin *w) {}
void GLWin::sectionUp(GLWin *w) {}
void GLWin::sectionDown(GLWin *w) {}

// TODO: this function requires passing a parameter telling us what sound to
// play
void GLWin::playSound(GLWin *w, const char soundName[]) {}
void GLWin::stopSound(GLWin *w) {}

uint32_t GLWin::internalRegisterAction(const char name[], Security s,
                                       Action action) {
  uint32_t securityIndex = uint32_t(s);
  // SAFE = 0..999, RESTRICTED=1000.1999, ASK=2000..2999
  uint32_t actNum = 1000 * securityIndex + numActions[securityIndex]++;
  // TODO: do something if 1000 action functions exceeded. For now, completely
  // unnecessary
  if (numActions[securityIndex] > 1000) {
    cerr << "Error! action Table is full for security " << securityIndex
         << '\n';
  }
  cout << "Setting action " << actNum << " for action " << name << '\n';
  setAction(actNum, action);
  actionNameMap[name] = actNum;
  return actNum;
}

unordered_map<string, int> GLWin::actionNameMap;

uint32_t GLWin::lookupAction(const char actionName[]) {
  auto it = actionNameMap.find(actionName);

  if (it == actionNameMap.end()) {  // throw Ex1(Errcode::NONEXISTENT_ACTION);
    cerr << "Input binding failed: " << actionName << '\n';
    return 0;
  }
  return it->second;
}

void GLWin::bind(uint32_t input, const char actionName[]) {
  setEvent(input, lookupAction(actionName));
}

void GLWin::bind2DOrtho() {
  bind(Inputs::LARROW, "panLeft2D");
  bind(Inputs::RARROW, "panRight2D");
  bind(Inputs::UPARROW, "panUp2D");
  bind(Inputs::DOWNARROW, "panDown2D");
  bind(Inputs::PAGEUP, "zoomIn2D");
  bind(Inputs::PAGEDOWN, "zoomOut2D");
}

void GLWin::bind3D() {
  bind(Inputs::INSERT, "speedTime");
  bind(Inputs::DEL, "slowTime");
  bind(Inputs::RARROW, "panRight3D");
  bind(Inputs::LARROW, "panLeft3D");
  bind(Inputs::PAGEUP, "zoomIn3D");
  bind(Inputs::PAGEDOWN, "zoomOut3D");
  //  bind(Inputs::MOUSE0|Inputs::PRESS|Inputs::ALT, "xyz");
}
void GLWin::loadBindings() {
  registerAction(Security::RESTRICTED, quit);
  registerAction(Security::SAFE, refresh);
  registerAction(Security::ASK, saveFrame);

  registerAction(Security::SAFE, gotoStartTime);
  registerAction(Security::SAFE, gotoEndTime);
  registerAction(Security::SAFE, speedTime);
  registerAction(Security::SAFE, slowTime);
  registerAction(Security::SAFE, resetTimeDilation);

  registerAction(Security::SAFE, resetProjection3D);
  registerAction(Security::SAFE, zoomOut3D);
  registerAction(Security::SAFE, zoomIn3D);
  registerAction(Security::SAFE, panRight3D);
  registerAction(Security::SAFE, panLeft3D);
  registerAction(Security::SAFE, panUp3D);
  registerAction(Security::SAFE, panDown3D);
  registerAction(Security::SAFE, selectObject3D);
  registerAction(Security::SAFE, addSelectObject3D);
  registerAction(Security::SAFE, toggleSelectObject3D);

  registerAction(Security::SAFE, resetProjection2D);
  registerAction(Security::SAFE, zoomOut2D);
  registerAction(Security::SAFE, zoomIn2D);
  registerAction(Security::SAFE, panRight2D);
  registerAction(Security::SAFE, panLeft2D);
  registerAction(Security::SAFE, panUp2D);
  registerAction(Security::SAFE, panDown2D);

  registerAction(Security::SAFE, gotoTop);
  registerAction(Security::SAFE, gotoBottom);
  registerAction(Security::SAFE, scrollUp);
  registerAction(Security::SAFE, scrollDown);
  registerAction(Security::SAFE, pageUp);
  registerAction(Security::SAFE, pageDown);
  registerAction(Security::SAFE, sectionUp);
  registerAction(Security::SAFE, sectionDown);

  // TODO: How to define actions that take parameters, in this case a string?
  //  registerAction(Security::SAFE, playSound);
  registerAction(Security::SAFE, stopSound);

  bind3D();
  // bind2DOrtho();
}

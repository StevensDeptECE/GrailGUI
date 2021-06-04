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

// uint32_t GLWin::inputMap[32768];
// GLWin::Action* GLWin::actionMap[4096];

// Inputs *GLWin::inp;
/* Find a better way to do this */
uint32_t GLWin::block_render = 0;
uint32_t GLWin::render_done = 0;

/* Booleans */
// uint32_t GLWin::use_inputs_library = 0;
uint32_t GLWin::mouse_entered = 0;
uint32_t GLWin::mouse_leaved = 1;
uint32_t GLWin::window_focused = 0;

/* GLFW Callbacks */

void GLWin::cursorPositionCallback(GLFWwindow *win, double xpos, double ypos) {
  GLWin *w = GLWin::getWin(win);
  if (w == nullptr) {
    cerr << "shouldn't be possible, but window is null! Uggh!\n";
    return;
  }
  glfwGetCursorPos(win, &w->mouseXPos, &w->mouseYPos);
}

void GLWin::cursorEnterCallback(GLFWwindow *win, int entered) {
  GLWin *w = GLWin::getWin(win);
  if (entered) {
    /* Update mouse position and
     * disable X11 cursor */
    glfwGetCursorPos(win, &w->mouseXPos, &w->mouseYPos);
    GLWin::mouse_leaved = 0;
    GLWin::mouse_entered = 1;
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    /* Mouse has left window
     * Enable X11 cursor */
    /* Not sure if needed anymore */
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    GLWin::mouse_leaved = 1;
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

void GLWin::key_callback(GLFWwindow *win, int key, int scancode, int action,
                         int mods) {
  GLWin *w = winMap[win];
  if (w == nullptr) {  //TODO: come up with better response? Should never happen
    cerr << "no mapping for window " << win << " to GLWin\n";
    return;
  }
  cerr << "key: " << key << " mods: " << mods << '\n';
  uint32_t act = GLWin::inputMap[key];
  if (act == 0) return;
  Action a = GLWin::actionMap[act];
  a(w);  // execute the action
}

void GLWin::mouseButtonCallback(GLFWwindow *win, int button, int action,
                                int mods) {
  GLWin *w = getWin(win);
  if (w == nullptr) {  //TODO: come up with better response? Should never happen
    cerr << "no mapping for window " << win << " to GLWin\n";
    return;
  }
  //find the button and click on it.
  //  ((Grail*)this) ->currentTab()->processMouseEvent
  //bool consumed = gui.clickOnObject(button, (uint32_t)mouseXPos, (uint32_t)mouseYPos);
  cerr << "mouse! " << button << "," << w->mouseXPos << "," << w->mouseYPos << '\n';
  //if (consumed)
  return;

  //	w->addMouseEvent(button, action, mods);
  //((GLFWInputs*)GLWin::inp)->setMouseState(button, action);
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

// Static initlializer for libraries
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
  glfwSetKeyCallback(win, key_callback);
  glfwSetWindowRefreshCallback(win, GLWin::windowRefreshCallback);

  // glEnable(GL_CULL_FACE); I disable this because when we change the
  // projection to be normal screen pixels than it doesnt draw since its drawing
  // it in the opposite orientation i assume
  glEnable(GL_BLEND);
  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_TEXTURE);
  glDepthFunc(GL_NEVER);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLWin::projection = glm::ortho(0.0f, static_cast<GLfloat>(width),
                                 static_cast<GLfloat>(height), 0.0f);
  //std::cerr << width << " " << height << std::endl;
  // this test is designed to make sure that multiple windows will only initialize fonts once
  // baseDir is not as serious, but why not do any singleton initialization here
  //TODO: is there any more elegant way?
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
  while (!glfwWindowShouldClose(win)) {
    //    bool modified = Queue::dump_render();
    //    dt = current - lastFrame;
    glfwPollEvents();  // Check and call events

    float startRender = glfwGetTime();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);  // Clear the colorbuffer and depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();
    renderTime += glfwGetTime() - startRender;
    glfwSwapBuffers(win);  // Swap buffer so the scene shows on screen
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
  exit(0);  //TODO: check for cleanup first?
}

void GLWin::refresh(GLWin *w) {
  //TODO: send signal to force redraw
}

void GLWin::saveFrame(GLWin *w) {
  if (w->saveBuffer != nullptr && w->saveW != w->width || w->saveH != w->height) {
    delete[] w->saveBuffer;
    w->saveBuffer = nullptr;
  }
  if (w->saveBuffer == nullptr) {
    strncpy(w->frameName, "tmp/frame", sizeof(w->frameName));
    w->saveW = w->width, w->saveH = w->height;
    w->saveBuffer = new uint8_t[3 * w->saveW * w->saveH];
  }
  sprintf(w->frameName + 8, "%d.png", w->frameNum++);
  glReadPixels(0, 0, w->saveW, w->saveH, GL_RGB, GL_UNSIGNED_BYTE, w->saveBuffer);
  constexpr int CHANNELS = 3;
  stbi_write_png(w->frameName, w->saveW, w->saveH, CHANNELS, w->saveBuffer,
                 w->saveW * CHANNELS);  // TODO: Remove - debugging purposes
}
void GLWin::resetCamera(GLWin *w) {
  //TODO: call resetCamera on every canvas to reset to initial state (we have to write that)
}

void GLWin::gotoStartTime(GLWin *w) {
  w->t = w->startTime;
}

void GLWin::gotoEndTime(GLWin *w) {
  w->t = w->endTime;
}

void GLWin::speedTime(GLWin *w) {
  w->dt *= 2;
}
void GLWin::slowTime(GLWin *w) {
  w->dt *= 0.5;
}

void GLWin::zoomOut(GLWin *w) {
}
void GLWin::zoomIn(GLWin *w) {
}

void GLWin::panRight(GLWin *w) {
}

void GLWin::panLeft(GLWin *w) {
}

void GLWin::loadBindings() {
  setAction(1000, speedTime);
  setAction(1001, slowTime);
  setAction(1002, zoomOut);
  setAction(1003, zoomIn);
  setAction(1004, panRight);
  setAction(1005, panLeft);
  setAction(1006, gotoStartTime);
  setAction(1007, gotoEndTime);
  setAction(1008, saveFrame);

  setEvent(260, 1000);  //INSERT->speed up time
  setEvent(261, 1001);  //DEL-> slow down time
  setEvent(262, 1004);  // right arrow = pan right
  setEvent(263, 1005);  // left arrow = pan left
  setEvent(266, 1002);  // page up = zoom out
  setEvent(267, 1003);  // page down = zoom in
  setEvent(335, 1008);  // ` is printscreen (printscreen seems taken by the OS?)
}

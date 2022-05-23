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

#include "csp/IPV4Socket.hh"
#include "csp/csp.hh"
#include "util/Prefs.hh"

#ifdef _WIN32
#include <windows.h>
#endif
#include <cstring>

#include "fmt/core.h"
#include "opengl/Style.hh"
#include "opengl/Tab.hh"
#include "stb/stb_image_write.h"
#include "xdl/XDLCompiler.hh"
#include "xdl/std.hh"
using namespace std;
string GLWin::baseDir;

unordered_map<GLFWwindow *, GLWin *> GLWin::winMap;
default_random_engine gen;
uniform_real_distribution<double> u01(0.0, 1.0);
glm::mat4 GLWin::projection;

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

void GLWin::keyCallback(GLFWwindow *win, int key, int scancode, int action,
                        int mods) {
  uint32_t input = (mods << 11) | (action << 9) | key;
  cerr << "key: " << key << " mods: " << mods << " input=" << input << '\n';
  winMap[win]->currentTab()->doit(input);
}

void GLWin::mouseButtonCallback(GLFWwindow *win, int button, int action,
                                int mods) {
  // find the button and click on it.
  //  ((Grail*)this) ->currentTab()->processMouseEvent
  // bool consumed = gui.clickOnObject(button, (uint32_t)mouseXPos,
  // (uint32_t)mouseYPos);
  GLWin *w = winMap[win];
  uint32_t input = (mods << 9) | (action << 3) | button;
  fmt::print("mouse!{}, action={}, location=({}, {}), input={}\n", button,
             action, w->mouseX, w->mouseY, input);
  winMap[win]->currentTab()->doit(input);
}

void GLWin::scrollCallback(GLFWwindow *win, double xoffset, double yoffset) {
  // cout << "xoffset=" << xoffset << " yoffset=" << yoffset << '\n';
  // todo: we would have to copy offsets into the object given the way this is
  uint32_t input = 400;
  winMap[win]->currentTab()->doit(input + int(yoffset));
}

void GLWin::windowRefreshCallback(GLFWwindow *win) {
  GLWin *w = GLWin::getWin(win);
  glfwGetWindowSize(win, (int *)&w->width, (int *)&w->height);
  w->needsRender = true;
}

void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam) {
  fprintf(stderr,
          "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
          (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type, severity,
          message);
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

inline glm::vec4 uint2vec4(uint32_t color) {
  return glm::vec4((color >> 24) / 255.0f, ((color >> 16) & 0xFF) / 255.0f,
                   ((color >> 8) & 0xFF) / 255.0f, (color & 0xFF) / 255.0f);
}

GLWin::GLWin(uint32_t bgColor, uint32_t fgColor, const string &title,
             uint32_t exitAfter)
    : bgColor(uint2vec4(bgColor)),
      fgColor(uint2vec4(fgColor)),
      title(title),
      exitAfter(exitAfter),
      tabs(4),
      faces(16),
      dragMode(false),
      mousePressX(0),
      mousePressY(0) {
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
  if (!ranStaticInits) GLWin::classInit();
}
bool GLWin::ranStaticInits = false;
bool GLWin::hasBeenInitialized = false;
GLWin::GLWin(uint32_t w, uint32_t h, uint32_t bgColor, uint32_t fgColor,
             const string &title, uint32_t exitAfter)
    : GLWin(bgColor, fgColor, title, exitAfter) {
  setSize(w, h);
  startWindow();
}

void GLWin::setTitle(const std::string &title) {}
MainCanvas *GLWin::getMainCanvas() { return currentTab()->getMainCanvas(); }

void GLWin::startWindow() {
  win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
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

  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
    baseDir = prefs.getBaseDir();
    FontFace::initAll();
  }
  defaultFont = (Font *)FontFace::get("TIMES", 40, FontFace::BOLD);
  Font *bigFont = (Font *)FontFace::get("TIMES", 20, FontFace::BOLD);
  Font *normalFont = (Font *)FontFace::get("TIMES", 12, FontFace::NORMAL);
  guiFont = bigFont;
  menuFont = bigFont;

  defaultStyle = new Style(defaultFont, 0.5, 0.5, 0.5, 0, 0, 0, 1,
                           COMMON_SHADER);  // added default line width
  defaultStyle->setLineWidth(1);
  //  defaultStyle->setShaderIndex(COMMON_SHADER);
  guiStyle = new Style(guiFont, 0.5f, 0.5f, 0.5f, 0, 0, 0, 1, COMMON_SHADER);
  guiTextStyle = new Style(guiFont, 0.5, 0.5, 0.5, 0, 0, 0, 1, COMMON_SHADER);
  menuStyle = new Style(menuFont, 0.5, 0.5, 0.5, 0, 0, 0, 1, COMMON_SHADER);
  menuTextStyle = new Style(menuFont, 0.5, 0.5, 0.5, 0, 0, 0, 1, COMMON_SHADER);
  tabs.add(new Tab(this));
  current = 0;
  hasBeenInitialized = true;
}
void GLWin::baseInit() {
  glLineWidth(1);
  Shader::setDir(prefs.getShaderDir());
  Shader::load("solid.bin", "common.vert", "common.frag");  // Solid Color
  Shader::load("pervert.bin", "vColor.vert",
               "common.frag");                         // Color per vertex
  Shader::load("text.bin", "text.vert", "text.frag");  // Texture for text
  Shader::load("img.bin", "Texture.vert",
               "Texture.frag");  // Texture for images
  Shader::load("cursor.bin", "Cursor.vert",
               "common.frag");  // Texture for images
  Shader::load("multiText.bin", "MultiTexture.vert",
               "MultiTexture.frag");  // MultiTexture for shapes
#if 0
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(messageCallback, 0);
#endif

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
void GLWin::render() { currentTab()->render(); }
// default is no animation. Override if you want your class to animate
void GLWin::update() { currentTab()->update(); }

// declare prototype access to static function in other file which calls
// FaceFont::cleanup()
void FontFaceCleanup();

GLWin::~GLWin() {
  cerr << "GLWin Destructor" << endl;
  GLWin::classCleanup();
}

// void GLWin::addFontPath(std::string path, std::string name) {
//   name.erase(name.end() - 4, name.end());

//   pathByName[name] = path;
//   // std::cout<<"Added: " << name.c_str()<<std::endl;
// }

void GLWin::mainLoop() {
  needsRender = true;
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
    float startRender = glfwGetTime();
    lastRenderTime = startRender;

    if (needsRender) {
      glClearColor(bgColor.r, bgColor.g, bgColor.b,
                   bgColor.a);  // Clear the colorbuffer and depth
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
      needsRender = false;
    }
    currentTab()->tick();  // update time in current tab for any models using
                           // simulation time
    needsUpdate = false;
    glfwPollEvents();  // Check and call events
    // note: any events needing a refresh should set dirty = true
    if (currentTab()->checkUpdate()) setUpdate();
    if (needsUpdate) {
      update();
      needsRender = true;
    } else {
      usleep(10);
    }
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

void GLWin::quit() {
  exit(0);  // TODO: check for cleanup first?
}

void GLWin::refresh() { setUpdate(); }

void GLWin::saveFrame() {
  if (saveBuffer != nullptr && saveW != width || saveH != height) {
    delete[] saveBuffer;
    saveBuffer = nullptr;
  }
  if (saveBuffer == nullptr) {
    strncpy(frameName, "tmp/frame", sizeof(frameName));
    saveW = width, saveH = height;
    saveBuffer = new uint8_t[3 * saveW * saveH];
  }
  sprintf(frameName + 8, "%d.png", frameNum++);
  glReadPixels(0, 0, saveW, saveH, GL_RGB, GL_UNSIGNED_BYTE, saveBuffer);
  constexpr int CHANNELS = 3;
  stbi_write_png(frameName, saveW, saveH, CHANNELS, saveBuffer,
                 saveW * CHANNELS);  // TODO: Remove - debugging purposes
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

void GLWin::nextTab() {
  if (current < tabs.size() - 1)
    current++;
  else
    current = 0;
}

void GLWin::prevTab() {
  if (current > 0)
    current--;
  else
    current = tabs.size() - 1;
}

Tab *GLWin::addTab() {
  Tab *newTab = new Tab(this);
  tabs.add(newTab);
  current = tabs.size() - 1;
  return newTab;
}

// TODO: write this, also consider writing a remove function for DynArray
void GLWin::removeTab() {}

void GLWin::goToLink(const char ipaddr[], uint16_t port, uint32_t requestID) {
  IPV4Socket s(ipaddr, port);
  s.send(requestID);
  Buffer &in = s.getIn();
  XDLCompiler compiler("");
  SymbolTable *st = new SymbolTable(&compiler);
  const XDLType *metadata = XDLType::readMeta(&compiler, in);
  Buffer out("client.txt", 32768);

  metadata->display(in, out);
}

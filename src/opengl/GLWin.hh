#pragma once

#include <stdlib.h>

#include <array>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>

#include "opengl/Colors.hh"
#include "opengl/GLWinFonts.hh"
#include "opengl/Shader.hh"
#include "util/DynArray.hh"
#include "util/HashMap.hh"
class GLFWwindow;  // forward declaration, simplify: include file not needed
                   // here
class GLWin;
typedef void (*Action)(GLWin*);  // actions are methods of the window
class Tab;
class Style;
class Font;
class XDLIterator;
class MainCanvas;

class GLWin {
 protected:
  Style* defaultStyle;
  Style* guiStyle;
  Style* guiTextStyle;
  Style* menuStyle;
  Style* menuTextStyle;
  Font* defaultFont;
  Font* guiFont;
  Font* menuFont;

 public:
  constexpr static uint32_t COMMON_SHADER = 0;
  constexpr static uint32_t PER_VERTEX_SHADER = 1;
  constexpr static uint32_t TEXT_SHADER = 2;
  constexpr static uint32_t TEXTURE_SHADER = 3;
  constexpr static uint32_t CURSOR_SHADER = 4;
  constexpr static uint32_t MULTI_TEXTURE_SHADER = 5;

 private:
  static bool ranStaticInits;
  static bool hasBeenInitialized;
  static std::unordered_map<GLFWwindow*, GLWin*> winMap;
  std::string title;
  uint8_t* saveBuffer;  // buffer used to save screenshots
  uint32_t saveW, saveH;
  uint32_t frameNum;
  double lastRenderTime;  // Stores last time of render
  char frameName[32];
  DynArray<Tab*> tabs;  // list of web pages, ie tabs
  uint32_t current;     // current (active) tab
  void checkUpdate();

 public:
  static std::string baseDir;
  double mouseX, mouseY;
  float mousePressX, mousePressY;
  bool dragMode;
  int winXPos, winYPos;    // location of the top-left of the window in pixels
  uint32_t width, height;  // width and height of the window in pixels
  bool needsUpdate, needsRender;
  bool focused;
  uint32_t exitAfter;  // if not zero, will terminate

 private:
  GLFWwindow* win;
  glm::vec4 bgColor, fgColor;
  DynArray<FontFace> faces;

  static GLWin* getWin(GLFWwindow* win) {
    std::unordered_map<GLFWwindow*, GLWin*>::iterator i = winMap.find(win);
    if (i == winMap.end())
      return nullptr;
    else
      return i->second;
  }
  static void doit(GLWin* w, uint32_t input);
  static void mouseButtonCallback(GLFWwindow* win, int button, int action,
                                  int mods);
  static void cursorPositionCallback(GLFWwindow* win, double xpos, double ypos);
  static void cursorEnterCallback(GLFWwindow* win, int entered);
  static void windowPositionCallback(GLFWwindow* win, int xpos, int ypos);
  static void windowFocusCallback(GLFWwindow* win, int focused);
  static void keyCallback(GLFWwindow* win, int key, int scancode, int action,
                          int mods);
  static void scrollCallback(GLFWwindow* win, double xoffset, double yoffset);

  static void windowRefreshCallback(GLFWwindow* win);
  static void enableMouse();

  static void resize(GLFWwindow* win, int width, int height);
  void processInput(GLFWwindow* win);
  static glm::mat4 projection;  // current projection for this window

  /*
 fontInit() and fontCleanup() are separated from the constructor/destructor into
 GLWinFonts to keep all the font-dependent code over there. Just call fontInit()
 before the action starts and fontCleanup() after the code is over.
 TODO: moved fontCleanup into FaceFont, should refactor fontInit to init() in
 faceFont as well.
 */

 public:
  // static std::unordered_map<std::string, std::string> pathByName;
  GLWin(uint32_t bgColor = 0x000000FF, uint32_t fgColor = 0xFFFFFFFF,
        const std::string& title = "", uint32_t exitAfter = 0);
  // 0xRRGGBBaa = color (RGB, alpha = opacity) possible source of bugs
  //! need to call setSize, startWindow manually
  // exitAfter150Frames: for debugging/benchmarking purposes
  GLWin(uint32_t w, uint32_t h, uint32_t bgColor, uint32_t fgColor,
        const std::string& title, uint32_t exitAfter = 0);

  void setTitle(const std::string& title);
  virtual ~GLWin();
  static int init(GLWin* g, uint32_t w, uint32_t h, uint32_t exitAfter = 0);
  static int init(GLWin* g, uint32_t exitAfter = 0) {
    return init(g, 1024, 1024, exitAfter);
  }
  MainCanvas* getMainCanvas();

  Tab* currentTab() { return tabs[current]; }

  void setSize(uint32_t w, uint32_t h) {
    width = w;
    height = h;
  }

  uint32_t getWidth() const { return width; }
  uint32_t getHeight() const { return height; }

  static glm::mat4* getProjection() { return &projection; }
  virtual void init();
  void startWindow();

  virtual void render();
  void addFontPath(std::string path, std::string name);

  virtual void update();
  virtual void cleanup();
  /* Manage animation with calls. Setting a time t, every time a tick happens
all animation moves forward to the next time t (integer)
*/
  void setDesiredColor(const glm::vec3& c, float delta);

  // void setAnimation(Shape& s, glm::mat&) // transform s by matrix every tick

  void random(glm::vec3& v);  // set this vec3 to random (0..1) for each
  /*
void loadObject(Model& m, const char filename[]);
void saveImage(Image& m); // save screenshot
Shape* pick(int x, int y); // click on (x,y) and get back frontmost shape
Shape* pick(int x, int y, Shape*); // click on (x,y), get Shape behind

*/
  void mainLoop();
  void setUpdate() { needsUpdate = true; }
  void setRender() { needsRender = true; }
  const Style* getDefaultStyle() const { return defaultStyle; }
  const Style* getGuiStyle() const { return guiStyle; }
  const Style* getGuiTextStyle() const { return guiTextStyle; }
  const Style* getMenuStyle() const { return menuStyle; }
  const Style* getMenuTextStyle() const { return menuTextStyle; }

  const Font* getDefaultFont() const { return defaultFont; }
  const Font* getGuiFont() const { return guiFont; }
  const Font* getMenuFont() const { return menuFont; }

  virtual void baseInit();

  static void classInit();
  static void classCleanup();

  void bind2DOrtho();
  void bind3D();

  // build in actions
  void quit();
  void refresh();
  void saveFrame();
  void resetCamera();
  void nextTab();
  void prevTab();
  Tab* addTab();
  void removeTab();
  void goToLink(const char ipaddr[], uint16_t port, uint32_t requestID);
};

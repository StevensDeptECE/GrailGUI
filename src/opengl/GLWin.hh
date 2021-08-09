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
  const char* title;
  double startTime;     // start of simulation time, default 0
  double endTime;       // end of simulation time, default 0
  double t;             // master time variable for animations
  double dt;            // delta time advanced every frame
  uint8_t* saveBuffer;  // buffer used to save screenshots
  uint32_t saveW, saveH;
  uint32_t frameNum;
  double lastRenderTime;  // Stores last time of render
  double lastUpdateTime;  // Stores last time of update
  double updateTime;      // Stores time between animation frames
  char frameName[32];
  DynArray<Tab*> tabs;  // list of web pages, ie tabs
  Tab* current;         // current (active) tab
  void checkUpdate();

 public:
  static std::string baseDir;
  double mouseX, mouseY;
  float mousePressX, mousePressY;
  bool dragMode;
  int winXPos, winYPos;    // location of the top-left of the window in pixels
  uint32_t width, height;  // width and height of the window in pixels
  bool dirty, needsRender;
  bool focused;
  uint32_t exitAfter;  // if not zero, will terminate

  float getTime() const;

  enum Inputs {
    INSERT = 260,
    DEL = 261,
    RARROW = 262,
    LARROW = 263,
    UPARROW = 265,
    DOWNARROW = 264,
    PAGEUP = 266,
    PAGEDOWN = 267,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    WHEELUP = 401,
    WHEELDOWN = 399,
    MOUSE0 = 0,
    MOUSE1 = 1,
    MOUSE2 = 2,
    MOUSE3 = 3,
    MOUSE4 = 4,
    KEY_RELEASE = 512,
    KEY_REPEAT = 1024,
    PRESS = 8,
    RELEASE = 0,
    CTRL = 2048,
    SHIFT = 4096,
    ALT = 8192
  };

  /*
        map input events, like pressing a key, or clicking alt-mouse button 1
     to an action number. Actions are internally stored in a separate table
     this allows defining key bindings
        TODO: Later, build a more sophisticated modal input where this can
     change in a mode, a second input map would override the first, allowing
     different mappings for different states.
  */
  inline static std::array<uint32_t, 32768> inputMap;
  /*
        map an integer code to a function to execute
        the actions are all the publicly available performance the code can DO
  */
  inline static std::array<std::function<void()>, 4096> actionMap;
  static HashMap<uint32_t> actionNameMap;
  uint32_t lookupAction(const char actionName[]);
  //	static GLWin* w;
  void setEvent(uint32_t e, uint32_t a) { inputMap[e] = a; }

  void setEvent(uint32_t key, uint32_t mod, uint32_t a) {
    setEvent((mod << 9) | key, a);
  }

  void setAction(uint32_t a, std::function<void()> action) {
    GLWin::actionMap[a] = action;
  }
  enum class Security {
    SAFE,        // safe for a remote server to trigger this function
    RESTRICTED,  // only the local user can execute this function
    ASK  // a remote user or server may request this but it will trigger a popup
         // asking local user to approve
  };
  std::array<uint32_t, 3> numActions;  // keep track of how many of each kind of
                                       // operations there are
  void loadBindings();
  uint32_t internalRegisterAction(const char name[], Security s,
                                  void (GLWin::*action)());
  uint32_t internalRegisterAction(const char name[], Security s,
                                  std::function<void()> action);
  // registerAction("myFunc", Security::RESTRICTED, myFunc)
  // bind an input event to an action Name. looks up offsets into arrays
  void bind(uint32_t input, const char actionName[]);
  void bind(const char inputCmd[], const char actionName[]);
#define quote(a) #a
#define registerAction(security, func) \
  internalRegisterAction(quote(func), security, func)
#define bindEvent(inp, func, ptr) \
  registerCallback(inp, quote(func), Security::SAFE, func, ptr)
  // Render control
  //  static uint32_t render_done, block_render;
  double time() const { return t; }
  void setDt(double delta) { dt = delta; }

 private:
  GLFWwindow* win;
  uint32_t bgColor, fgColor;
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
        const char title[] = nullptr,
        uint32_t exitAfter =
            0);  // 0xRRGGBBaa = color (RGB, alpha =
                 // opacity) possible source of bugs
                 //! need to call setSize, startWindow manually
                 // exitAfter150Frames: for debugging/benchmarking purposes
  GLWin(uint32_t w, uint32_t h, uint32_t bgColor, uint32_t fgColor,
        const char title[],
        uint32_t exitAfter = 0);  // use this constructor for standalone
                                  // to build window (all in one)

  virtual ~GLWin();
  static int init(GLWin* g, uint32_t w, uint32_t h, uint32_t exitAfter = 0);
  static int init(GLWin* g, uint32_t exitAfter = 0) {
    return init(g, 1024, 1024, exitAfter);
  }

  Tab* currentTab() { return current; }

  void setSize(uint32_t w, uint32_t h) {
    width = w;
    height = h;
  }

  uint32_t getWidth() const { return width; }
  uint32_t getHeight() const { return height; }
  void setFrameRate(double framerate) {
    updateTime = (framerate == 0) ? 0 : (1.0 / framerate);
  }

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
  void resetAnim();          // set t=0
  void setTime(float t);     // set t to any desired value
  void tick();               // move t forward
  void setEndTime(float t);  // define the end time. When end is reached restart
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
  void setDirty() { dirty = true; }
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

  uint32_t registerCallback(uint32_t input, const char name[], Security s,
                            void (GLWin::*callback)());
  uint32_t registerCallback(uint32_t input, const char name[], Security s,
                            std::function<void()> action);
  template <typename T, typename U>
  uint32_t registerCallback(uint32_t input, const char name[], Security s,
                            void (T::*callback)(), U* ptr) {
    auto cb_funcptr = std::bind(callback, ptr);
    return registerCallback(input, name, s, cb_funcptr);
  }
  void bind2DOrtho();
  void bind3D();

  // build in actions
  void quit();
  void refresh();
  void saveFrame();
  void resetCamera();

  void gotoStartTime();
  void gotoEndTime();
  void speedTime();
  void slowTime();
  void resetTimeDilation();

  void clearSelected(GLWin* w);

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

  void resetProjection2D();
  void zoomOut2D();
  void zoomIn2D();
  void panRight2D();
  void panLeft2D();
  void panUp2D();
  void panDown2D();

  void gotoTop();
  void gotoBottom();
  void scrollUp();
  void scrollDown();
  void pageUp();
  void pageDown();
  void sectionUp();
  void sectionDown();

  static void pressOnWidget(GLWin* w);
  static void releaseWidget(GLWin* w);

  static void clickOnWidget(GLWin* w);
  static void helloWorld(GLWin* w);

  double getTime();
};

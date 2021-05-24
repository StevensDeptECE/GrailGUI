#pragma once

#include <stdlib.h>

#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>
#include "util/DynArray.hh"

#include "opengl/GLWinFonts.hh"
#include "opengl/Shader.hh"
#include "opengl/Colors.hh"
class GLFWwindow;  // forward declaration, simplify: include file not needed
                   // here
class GLWin;
typedef void (*Action)(GLWin *);  // actions are methods of the window
class Tab;
class Style;
class Font;
class GLWin {
 protected:
  Style* defaultStyle;
  Style* guiStyle;
  Style* guiTextStyle;
  Style* menuStyle;
  Style* menuTextStyle;
  Font *defaultFont;
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
  static bool hasBeenInitialized;
  static std::unordered_map<GLFWwindow *, GLWin *> winMap;
  const char *title;
  double startTime;       // start of simulation time, default 0
  double endTime;         // end of simulation time, default 0
  double t;               // master time variable for animations
  double dt;              // delta time advanced every frame
  uint8_t* saveBuffer;    // buffer used to save screenshots
  uint32_t saveW, saveH;
  uint32_t frameNum;
  char frameName[32]; 
  DynArray<Tab*> tabs;    // list of web pages, ie tabs
  Tab* current;           // current (active) tab
 public:
  static std::string baseDir;
  double mouseXPos, mouseYPos;
  int winXPos, winYPos;   // location of the top-left of the window in pixels
  uint32_t width, height; // width and height of the window in pixels
  bool dirty;
  bool focused;
  uint32_t exitAfter;     // if not zero, will terminate
  /*
        map input events, like pressing a key, or clicking alt-mouse button 1
     to an action number. Actions are internally stored in a separate table
     this allows defining key bindings
        TODO: Later, build a more sophisticated modal input where this can
     change in a mode, a second input map would override the first, allowing
     different mappings for different states.
  */
  static uint32_t inputMap[32768];
  /*
        map an integer code to a function to execute
        the actions are all the publicly available performance the code can DO
  */
  static Action actionMap[4096];  // TODO: hardcoded maximum size for now
  //	static GLWin* w;
  void setEvent(uint32_t e, uint32_t a) { inputMap[e] = a; }

  void setEvent(uint32_t key, uint32_t mod, uint32_t a) {
    setEvent((mod << 9) | key, a);
  }

  void setAction(uint32_t a, Action action) { actionMap[a] = action; }

  /* Stuff related to Mouse and Input */
  // TODO: Clean this up, it really needs it
  static double m_xpos, m_ypos;  // Mouse position
  static int w_xpos, w_ypos;     // Window position
  static uint32_t mouse_entered, mouse_leaved, window_focused;
  //  static uint32_t use_inputs_library;
  static void getCurrentLocation();

  // Render control
  static uint32_t render_done, block_render;
  double time() const { return t; }
  void setDt(double delta) { dt = delta; }
 private:
  GLFWwindow *win;
  uint32_t bgColor, fgColor;
  DynArray<FontFace> faces;

  static GLWin *getWin(GLFWwindow *win) {
    std::unordered_map<GLFWwindow *, GLWin *>::iterator i = winMap.find(win);
    if (i == winMap.end())
      return nullptr;
    else
      return i->second;
  }
  static void mouseButtonCallback(GLFWwindow *win, int button, int action,
                                  int mods);
  static void cursorPositionCallback(GLFWwindow *win, double xpos, double ypos);
  static void cursorEnterCallback(GLFWwindow *win, int entered);
  static void windowPositionCallback(GLFWwindow *win, int xpos, int ypos);
  static void windowFocusCallback(GLFWwindow *win, int focused);
  static void key_callback(GLFWwindow *win, int key, int scancode, int action,
                           int mods);
  static void windowRefreshCallback(GLFWwindow *win);
  static void enableMouse();

  static void resize(GLFWwindow *win, int width, int height);
  void processInput(GLFWwindow *win);
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
        uint32_t exitAfter = 0);  // 0xRRGGBBaa = color (RGB, alpha =
                     // opacity) possible source of bugs
                     //! need to call setSize, startWindow manually
                     // exitAfter150Frames: for debugging/benchmarking purposes
  GLWin(
      uint32_t w, uint32_t h, uint32_t bgColor, uint32_t fgColor,
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
	
  static glm::mat4 *getProjection() { return &projection; }
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
  void setDesiredColor(const glm::vec3 &c, float delta);

  // void setAnimation(Shape& s, glm::mat&) // transform s by matrix every tick

  void random(glm::vec3 &v);  // set this vec3 to random (0..1) for each
  /*
void loadObject(Model& m, const char filename[]);
void saveImage(Image& m); // save screenshot
Shape* pick(int x, int y); // click on (x,y) and get back frontmost shape
Shape* pick(int x, int y, Shape*); // click on (x,y), get Shape behind

*/
  void mainLoop();
  void setDirty() { dirty = true; }
  const Style *getDefaultStyle() const { return defaultStyle; }
	const Style* getGuiStyle() const     { return guiStyle; }
	const Style* getGuiTextStyle() const { return guiTextStyle; }
	const Style* getMenuStyle() const     { return menuStyle; }
	const Style* getMenuTextStyle() const { return menuTextStyle; }
	
  const Font *getDefaultFont() const { return defaultFont; }
  const Font *getGuiFont() const     { return guiFont; }
  const Font *getMenuFont() const    { return menuFont; }

  virtual void baseInit();


  static void quit(GLWin* w);
  static void refresh(GLWin* w);
  static void saveFrame(GLWin* w);
  static void resetCamera(GLWin* w);
  static void gotoStartTime(GLWin* w);
  static void gotoEndTime(GLWin* w);
  static void speedTime(GLWin* w);
  static void slowTime(GLWin* w);
  static void resetTimeDilation(GLWin* w);
  static void zoomOut(GLWin* w);
  static void zoomIn(GLWin* w);
  static void panRight(GLWin* w);
  static void panLeft(GLWin* w);
  static void playSound(const char name[]);
  static void stopSound();
  void loadBindings();
};


#include "opengl/BlockMapLoader.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MapView2D.hh"

using namespace std;
using namespace grail;

class TestDrawBlockMap : public GLWin {
 private:
  const char* filename;
  MapView2D* mv;

 public:
  TestDrawBlockMap(const char filename[])
      : GLWin(0x000000, 0xCCCCCC, "Block Loader: Map Demo"),
        filename(filename),
        mv(nullptr) {}
  int countyStart;
  int numCounties;
  int displayNumCounties;

  ~TestDrawBlockMap() { delete mv; }
  TestDrawBlockMap(const TestDrawBlockMap& orig) = delete;
  TestDrawBlockMap& operator=(const TestDrawBlockMap& orig) = delete;

  constexpr static float zoom = 1.2;
  static void mapZoomIn(GLWin* w) {
    TestDrawBlockMap* map = ((TestDrawBlockMap*)w);
    glm::mat4& t = map->mv->getTransform();
    t = glm::translate(t, glm::vec3(-74, +40, 0));
    t = glm::scale(t, glm::vec3(zoom));
    t = glm::translate(t, glm::vec3(+74, -40, 0) / zoom);
  }

  static void mapZoomOut(GLWin* w) {
    TestDrawBlockMap* eml = ((TestDrawBlockMap*)w);
  }

  static void nextCounty(GLWin* w) {
    TestDrawBlockMap* eml = ((TestDrawBlockMap*)w);
    if (eml->countyStart < eml->numCounties) eml->countyStart++;
  }
  static void prevCounty(GLWin* w) {
    TestDrawBlockMap* eml = ((TestDrawBlockMap*)w);
    if (eml->countyStart > 0) eml->countyStart--;
  }
  static void displayAllCounties(GLWin* w) {
    TestDrawBlockMap* eml = ((TestDrawBlockMap*)w);
    eml->countyStart = 0;
    eml->displayNumCounties = eml->numCounties;
  }
  static void display3Counties(GLWin* w) {
    TestDrawBlockMap* eml = ((TestDrawBlockMap*)w);
    eml->displayNumCounties = 3;
  }
  static void decreaseCounties(GLWin* w) {
    TestDrawBlockMap* eml = ((TestDrawBlockMap*)w);
    if (eml->displayNumCounties > 0) eml->displayNumCounties--;
  }
  static void increaseCounties(GLWin* w) {
    TestDrawBlockMap* eml = ((TestDrawBlockMap*)w);
    if (eml->displayNumCounties < eml->numCounties) eml->displayNumCounties++;
  }

  // void transform(ESRIPoint& pt, double shiftX, double shiftY, double scaleX,
  // double scaleY) {
  // pt.x = pt.x * scaleX + shiftX;
  // pt.y = pt.y * scaleY + shiftY;
  //}

  void init() {
    MainCanvas* c = currentTab()->getMainCanvas();
    const Style* s = getDefaultStyle();
    Style* s2 = new Style(getDefaultFont(), grail::white, grail::black);
    mv = c->addLayer(new MapView2D(c, s2, new BlockMapLoader(filename)));

#if 0
    // Keybinds for animation!
    setAction(1002, displayAllCounties);
    setAction(1003, display3Counties);
    setAction(1004, nextCounty);
    setAction(1005, prevCounty);
    setAction(1006, increaseCounties);
    setAction(1007, decreaseCounties);
    setAction(1008, toggleAnimate);
    setAction(1009, zoomIn2D);
    setAction(1010, zoomOut2D);
    setAction(1011, panLeft2D);
    setAction(1012, panRight2D);
    setAction(1013, panUp2D);
    setAction(1014, panDown2D);
    setAction(1015, resetProjection2D);

    setEvent(266, 1002);  // page up = displayAll
    setEvent(267, 1003);  // page down = display3
    setEvent(262, 1004);  // right arrow = nextCount
    setEvent(263, 1005);  // left arrow = prevCounty
    setEvent(265, 1006);  // up arrow = increase  display
    setEvent(264, 1007);  // down arrow = decrease display
    setEvent(46, 1008);   // period = toggle animation
    setEvent(401, 1009);  // scroll up zoom in 2d
    setEvent(399, 1010);  // scroll down zoom in 2d
    setEvent('W', 1013);  // w PAN up
    setEvent('S', 1014);  // S PAN Down
    setEvent('A', 1011);  // c pan left
    setEvent('D', 1012);  // d pan right

    setEvent('Q', 1009);  // zoom in
    setEvent('E', 1010);  // zoom out
    setEvent('P', 1015);
#endif
    update();
  }

  void update() {}
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestDrawBlockMap("uscounties.bml"), 2000, 2000);
}

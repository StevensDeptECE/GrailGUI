
#include "opengl/BlockMapLoader.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MapView2D.hh"

using namespace std;
using namespace grail;

class TestDrawBlockMap : public GLWin {
 private:
  const char* filename;

 public:
  TestDrawBlockMap(const char filename[])
      : GLWin(0x000000, 0xCCCCCC, "Block Loader: Map Demo"),
        filename(filename) {}
  int countyStart;
  int numCounties;
  int displayNumCounties;

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
    MapView2D* mv =
        c->addLayer(new MapView2D(c, s, new BlockMapLoader(filename)));
    bind2DOrtho();

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
  return GLWin::init(new TestDrawBlockMap("uscounties.bml"), 1920, 1080);
}

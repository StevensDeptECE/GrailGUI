
#include "data/BlockMapLoader.hh"
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

  constexpr static float zoomVal = 1.2;
  static void mapZoomIn(GLWin* w) {
    TestDrawBlockMap* map = ((TestDrawBlockMap*)w);
    MapView2D* mv = map->mv;
    //    glm::mat4& t = mv->getTransform();
    mv->uniformZoom(1 / 1.2f);
    mv->setProjection();
    //    t = glm::translate(t, glm::vec3(-74, +40, 0));
    // t = glm::scale(t, glm::vec3(1.2f));
    // t = glm::translate(t, glm::vec3(+74, -40, 0) / 1.2f);
  }

  static void mapZoomOut(GLWin* w) {
    TestDrawBlockMap* map = ((TestDrawBlockMap*)w);
    MapView2D* mv = map->mv;
    mv->uniformZoom(1.2f);
    mv->setProjection();
    //    glm::mat4& t = map->mv->getTransform();
    // t = glm::translate(t, glm::vec3(-74, +40, 0));
    // t = glm::scale(t, glm::vec3(1 / 1.2f));
    // t = glm::translate(t, glm::vec3(+74, -40, 0) * 1.2f);
  }

  static void mapPanRight(GLWin* w) {
    TestDrawBlockMap* map = ((TestDrawBlockMap*)w);
    MapView2D* mv = map->mv;
    mv->translate(0.2, 0);
    mv->setProjection();
  }
  static void mapPanLeft(GLWin* w) {
    TestDrawBlockMap* map = ((TestDrawBlockMap*)w);
    MapView2D* mv = map->mv;
    mv->translate(-0.2, 0);
    mv->setProjection();
  }
  static void mapPanUp(GLWin* w) {
    TestDrawBlockMap* map = ((TestDrawBlockMap*)w);
    MapView2D* mv = map->mv;
    mv->translate(0, 0.2);
    mv->setProjection();
  }
  static void mapPanDown(GLWin* w) {
    TestDrawBlockMap* map = ((TestDrawBlockMap*)w);
    MapView2D* mv = map->mv;
    mv->translate(0, -0.2);
    mv->setProjection();
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
    bindEvent(Inputs::WHEELUP, mapZoomIn);
    bindEvent(Inputs::WHEELDOWN, mapZoomOut);
    bindEvent(Inputs::RARROW, mapPanRight);
    bindEvent(Inputs::LARROW, mapPanLeft);
    bindEvent(Inputs::UPARROW, mapPanUp);
    bindEvent(Inputs::DOWNARROW, mapPanDown);
    update();
  }

  void update() {}
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestDrawBlockMap("res/maps/uscounties.bml"), 2000,
                     2000);
}

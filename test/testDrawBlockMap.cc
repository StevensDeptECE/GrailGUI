
#include "data/BlockMapLoader.hh"
#include "opengl/ButtonWidget.hh"
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
  void mapZoomIn() {
    //    glm::mat4& t = mv->getTransform();
    mv->uniformZoom(1 / 1.2f);
    mv->setProjection();
    //    t = glm::translate(t, glm::vec3(-74, +40, 0));
    // t = glm::scale(t, glm::vec3(1.2f));
    // t = glm::translate(t, glm::vec3(+74, -40, 0) / 1.2f);
  }

  void mapZoomOut() {
    mv->uniformZoom(1.2f);
    mv->setProjection();
    //    glm::mat4& t = map->mv->getTransform();
    // t = glm::translate(t, glm::vec3(-74, +40, 0));
    // t = glm::scale(t, glm::vec3(1 / 1.2f));
    // t = glm::translate(t, glm::vec3(+74, -40, 0) * 1.2f);
  }

  void mapPanRight() {
    mv->translate(0.2, 0);
    mv->setProjection();
  }

  void mapPanLeft() {
    mv->translate(-0.2, 0);
    mv->setProjection();
  }

  void mapPanUp() {
    mv->translate(0, 0.2);
    mv->setProjection();
  }

  void mapPanDown() {
    mv->translate(0, -0.2);
    mv->setProjection();
  }

  void nextCounty() {
    if (countyStart < numCounties) countyStart++;
  }

  void prevCounty() {
    if (countyStart > 0) countyStart--;
  }

  void displayAllCounties() {
    countyStart = 0;
    displayNumCounties = numCounties;
  }

  void display3Counties() { displayNumCounties = 3; }

  void decreaseCounties() {
    if (displayNumCounties > 0) displayNumCounties--;
  }

  void increaseCounties() {
    if (displayNumCounties < numCounties) displayNumCounties++;
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
    c->addClickableWidget(
        new ButtonWidget(c, 0, 0, 200, 100, "Click Me!", "mapZoomIn"));
    mv = c->addLayer(new MapView2D(c, s2, new BlockMapLoader(filename)));

    bindEvent(Inputs::WHEELUP, &TestDrawBlockMap::mapZoomIn, this);
    bindEvent(Inputs::WHEELDOWN, &TestDrawBlockMap::mapZoomOut, this);
    bindEvent(Inputs::RARROW, &TestDrawBlockMap::mapPanRight, this);
    bindEvent(Inputs::LARROW, &TestDrawBlockMap::mapPanLeft, this);
    bindEvent(Inputs::UPARROW, &TestDrawBlockMap::mapPanUp, this);
    bindEvent(Inputs::DOWNARROW, &TestDrawBlockMap::mapPanDown, this);
    update();
  }

  void update() {}
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestDrawBlockMap("res/maps/uscounties.bml"), 2000,
                     2000);
}

#include "opengl/Animated.hh"
#include "data/BlockMapLoader.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MapView2D.hh"

using namespace std;
using namespace grail;

class TestDrawBlockMap : public Animated {
 private:
  const char* filename;
  MapView2D* mv;

 public:
  TestDrawBlockMap(Tab* tab, const char filename[])
		: Animated(tab), //GLWin(0x000000, 0xCCCCCC, "Block Loader: Map Demo"),
			filename(filename),
			mv(nullptr) {
    MainCanvas* c = tab->getMainCanvas();
		//    const Style* s = tab->getDefaultStyle();
    Style* s2 = new Style(tab->getDefaultFont(), grail::white, grail::black);
		//    c->addClickableWidget(
		//new ButtonWidget(c, 0, 0, 200, 100, "Click Me!", "mapZoomIn"));
    mv = c->addLayer(new MapView2D(c, s2, new BlockMapLoader(filename)));

    bindEvent(Tab::Inputs::WHEELUP, &TestDrawBlockMap::mapZoomIn, this);
    bindEvent(Tab::Inputs::WHEELDOWN, &TestDrawBlockMap::mapZoomOut, this);
    bindEvent(Tab::Inputs::RARROW, &TestDrawBlockMap::mapPanRight, this);
    bindEvent(Tab::Inputs::LARROW, &TestDrawBlockMap::mapPanLeft, this);
    bindEvent(Tab::Inputs::UPARROW, &TestDrawBlockMap::mapPanUp, this);
    bindEvent(Tab::Inputs::DOWNARROW, &TestDrawBlockMap::mapPanDown, this);
    update();
  }

			
  int countyStart;
  int numCounties;
  int displayNumCounties;

  ~TestDrawBlockMap() { delete mv; }
  TestDrawBlockMap(const TestDrawBlockMap& orig) = delete;
  TestDrawBlockMap& operator=(const TestDrawBlockMap& orig) = delete;

  constexpr static float zoomVal = 1.2;
  void mapZoomIn() {
    mv->uniformZoom(1 / 1.2f);
    mv->setProjection();
  }

  void mapZoomOut() {
    mv->uniformZoom(1.2f);
    mv->setProjection();
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
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* tab) {
	tab->addAnimated(new TestDrawBlockMap("res/maps/uscounties.bml"));
}

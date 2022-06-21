#include "data/BlockMapLoader2.hh"
#include "maps/MapNames.hh"
#include "maps/MapView2D.hh"
#include "opengl/ButtonWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/MultiText.hh"

using namespace std;
using namespace grail;

class TestDrawBlockMap : public Member {
 private:
  const char* filename;
  // both these components must be fed the same coordinates
  MapView2D* mv;  // map view  displays the map lines and filled regions
  MultiText* mt;  // multi text displays the text labels on top of the map

 public:
  TestDrawBlockMap(Tab* tab, const char bmlfile[], const char bdlfile[])
      : Member(tab),  // GLWin(0x000000, 0xCCCCCC, "Block Loader: Map Demo"),
        filename(bmlfile),
        mv(nullptr) {
    MapViewer* viewer = new MapViewer(getParent(), tab, 0, );
    viewer->setOrthoProjection(-180, 180, 0, 90);
    
    MainCanvas* c = tab->getMainCanvas();
    c->setOrthoProjection(-180, 180, 0, 90);
    //    const Style* s = tab->getDefaultStyle();
    Style* s2 = new Style(tab->getDefaultFont(), grail::white, grail::black);
    //    c->addClickableWidget(
    // new ButtonWidget(c, 0, 0, 200, 100, "Click Me!", "mapZoomIn"));
    BlockMapLoader* bml = new BlockMapLoader(bmlfile);
    BLHashMap<MapEntry>* bdl = new BLHashMap<MapEntry>(bdlfile);

    // TODO: MultiText is drawing using Map coordinates but with a projection of
    // web coordinates
    mt = c->addLayer(new MultiText(c, s2, 12));
    const Font* f = mt->getStyle()->f;
    mt->addCentered(50, 100, f, "testing");
    mv = c->addLayer(new MapView2D(c, s2, bml, bdl));

    cout << "num points loaded: " << bml->getNumPoints() << '\n';

    tab->bindEvent(Tab::Inputs::WHEELUP, &TestDrawBlockMap::mapZoomIn, this);
    tab->bindEvent(Tab::Inputs::WHEELDOWN, &TestDrawBlockMap::mapZoomOut, this);
    tab->bindEvent(Tab::Inputs::RARROW, &TestDrawBlockMap::mapPanRight, this);
    tab->bindEvent(Tab::Inputs::LARROW, &TestDrawBlockMap::mapPanLeft, this);
    tab->bindEvent(Tab::Inputs::UPARROW, &TestDrawBlockMap::mapPanUp, this);
    tab->bindEvent(Tab::Inputs::DOWNARROW, &TestDrawBlockMap::mapPanDown, this);
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

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  new TestDrawBlockMap(defaultTab, "res/maps/uscounties.bml",
                       "res/maps/uscounties.bdl");
}

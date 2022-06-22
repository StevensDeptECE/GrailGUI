#include "opengl/GrailGUI.hh"
#include "data/BlockMapLoader2.hh"
#include "util/BLHashMap.hh"
#include "maps/MapNames.hh"
//#include "maps/MapView2D.hh"
#include "maps/MapViewer.hh"
//#include "opengl/ButtonWidget.hh"
//#include "opengl/MultiText.hh"

using namespace std;
using namespace grail;

class TestDrawBlockMap : public Member {
 private:
  const char* filename;
  // both these components must be fed the same coordinates
  //MapView2D* mv;  // map view  displays the map lines and filled regions
  //MultiText* mt;  // multi text displays the text labels on top of the map
  MapViewer* viewer;
 public:
  TestDrawBlockMap(Tab* tab, const char bmlfile[], const char bdlfile[])
      : Member(tab),  // GLWin(0x000000, 0xCCCCCC, "Block Loader: Map Demo"),
        filename(bmlfile) {
    //    c->addClickableWidget(
    // new ButtonWidget(c, 0, 0, 200, 100, "Click Me!", "mapZoomIn"));
    Style* s = new Style(tab->getDefaultFont(), grail::white, grail::black);
    BlockMapLoader* bml = new BlockMapLoader(bmlfile);
    BLHashMap<MapEntry>* bdl = new BLHashMap<MapEntry>(bdlfile);
    viewer = new MapViewer(getParent(), tab, s, bml, bdl);
    viewer->setOrthoProjection(-180, -60, 20, 80);
    //viewer->setView();

    cout << "num points loaded: " << bml->getNumPoints() << '\n';
//TODO: Alice show us how to clean this up and use lambdas!
    tab->bindEvent(Tab::Inputs::WHEELUP, [=,this]() {return viewer->zoomIn(1.2f);});
    tab->bindEvent(Tab::Inputs::WHEELDOWN, [=,this]() { return viewer->zoomOut(1.2f); });
    tab->bindEvent(Tab::Inputs::RARROW, [=,this]() { return viewer->translate(0.2, 0); });
    tab->bindEvent(Tab::Inputs::LARROW, [=,this]() { return viewer->translate(-0.2, 0); });
    tab->bindEvent(Tab::Inputs::UPARROW, [=,this]() { return viewer->translate(0, 0.2); });
    tab->bindEvent(Tab::Inputs::DOWNARROW, [=,this]() { return viewer->translate(0, -0.2); });
    //TODO:
    update();
  }

  int countyStart;
  int numCounties;
  int displayNumCounties;

  ~TestDrawBlockMap() { delete viewer; }
  TestDrawBlockMap(const TestDrawBlockMap& orig) = delete;
  TestDrawBlockMap& operator=(const TestDrawBlockMap& orig) = delete;

  constexpr static float zoomVal = 1.2;

  void nextCounty() {
    if (countyStart < numCounties) countyStart++;
    viewer->setView();
  }

  void prevCounty() {
    if (countyStart > 0) countyStart--;
    viewer->setView();
  }

  void displayAllCounties() {
    countyStart = 0;
    displayNumCounties = numCounties;
    viewer->setView();
  }

  void display3Counties() {
     displayNumCounties = 3;
     viewer->setView();
 }

  void decreaseCounties() {
    if (displayNumCounties > 0) displayNumCounties--;
    viewer->setView();
  }

  void increaseCounties() {
    if (displayNumCounties < numCounties) displayNumCounties++;
    viewer->setView();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  new TestDrawBlockMap(defaultTab, "res/maps/uscounties.bml",
                       "res/maps/uscounties.bdl");
}

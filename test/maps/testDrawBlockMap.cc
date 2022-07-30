#include "opengl/GrailGUI.hh"
#include "data/BlockMapLoader.hh"
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
    viewer = new MapViewer(getParent(), tab, s, bml, bdl, 0.002);
    viewer->setOrigBounds(-90, 90, -180, 180);
    // BUG: bounds from bml are somehow corrupted, so we set the bounds ourselves

    cout << "num points loaded: " << bml->getNumPoints() << '\n';
    tab->bindEvent(Tab::Inputs::WHEELUP, [=,this]() {return viewer->zoomIn(1.2f);});
    tab->bindEvent(Tab::Inputs::WHEELDOWN, [=,this]() { return viewer->zoomOut(1.2f); });
    tab->bindEvent(Tab::Inputs::RARROW, [=,this]() { return viewer->translatePercent(0, 0.2); });
    tab->bindEvent(Tab::Inputs::LARROW, [=,this]() { return viewer->translatePercent(0, -0.2); });
    tab->bindEvent(Tab::Inputs::UPARROW, [=,this]() { return viewer->translatePercent(0.2, 0); });
    tab->bindEvent(Tab::Inputs::DOWNARROW, [=,this]() { return viewer->translatePercent(-0.2, 0); });
    tab->bindEvent(Tab::Inputs::MOUSE2, [=,this]() {return viewer->resetToOriginal();});
    tab->bindEvent(Tab::Inputs::INSERT, [=,this]() {return viewer->increaseTextSize(1.2f);});
    tab->bindEvent(Tab::Inputs::DEL, [=,this]() {return viewer->decreaseTextSize(1.2f);});
    tab->bindEvent(Tab::Inputs::HOME, [=,this]() {return viewer->toggleDisplayText();});
    tab->bindEvent(Tab::Inputs::PAGEUP, [=,this]() {return viewer->toggleDisplayOutline();});
    tab->bindEvent(Tab::Inputs::PAGEDOWN, [=,this]() {return viewer->toggleDisplayFill();});
    tab->bindEvent(Tab::Inputs::LCTRL, [=,this]() {return viewer->toggleDisplayCountyNames();});
    tab->bindEvent(Tab::Inputs::LALT, [=,this]() {return viewer->toggleDisplayStateNames();});
    tab->bindEvent(Tab::Inputs::MOUSE0_RELEASE, [=,this]() {return viewer->zoomInOnMouse(1.2f);});
    tab->bindEvent(Tab::Inputs::MOUSE1, [=,this]() {return viewer->zoomOutOnMouse(1.2f);});
    
    tab->bindEvent(Tab::Inputs::F1, [=,this]() {return viewer->displayAllSegments();});
    tab->bindEvent(Tab::Inputs::F2, [=,this]() {return viewer->incSegment();});
    tab->bindEvent(Tab::Inputs::F3, [=,this]() {return viewer->decSegment();});
    tab->bindEvent(Tab::Inputs::F4, [=,this]() {return viewer->incNumSegments();});
    tab->bindEvent(Tab::Inputs::F5, [=,this]() {return viewer->displayFirstSegment();});
    tab->bindEvent(Tab::Inputs::F6, [=,this]() {return viewer->displayState("Alaska");});
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

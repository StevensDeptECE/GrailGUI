#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include "opengl/GapMinderWidget.hh"
#include "data/GapMinderLoader.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;


class TestWidgets : public GLWin {
 public:

  GapMinderWidget *chart;
  double startTime;

  TestWidgets() : GLWin(0x000000, 0xCCCCCC, "TestWidgets") {}

  void testGapMinder(StyledMultiShape2D *gui, MultiText *guiText) {

    const uint32_t bigSize = 1024*1024;

    gui->reserve(bigSize, bigSize, bigSize, 1024);

    string xData = "gdp_per_capita.csv";
    string yData = "ddf--datapoints--poisonings_deaths_per_100000_people.csv";
    string sData = "ddf--datapoints--vacc_rate--by--country--time.csv";

    vector <float> x2 = {65000, 50000, 40000};
    vector <float> y2 = {14, 3, 5};
    vector <float> s2 = {80, 90, 95};
    vector <glm::vec4> c2 = {grail::blue, grail::green, grail::red};

    chart = new GapMinderWidget(gui, guiText, 75, 50, 900, 450, x2, y2);
    //chart->chart(y2, x2, s2, 10000, 2, c2);
    chart->loadData(yData, xData, sData, 2000, 2010, 10000, 2);

    chart->setTitle("Title");
    chart->init();
  }

  void update(){
    
    double time = getTime();
    if (time > startTime + 2 ) {
      startTime = time;
      chart->update();
      setDirty();
    }

  }


  void init() {

    startTime = getTime();

    const Style *s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text

    const Style *s2 = new Style("TIMES", 24, 1, 0, 1, 0, 0, 0, 0, 0, 0);

    MainCanvas *c = currentTab()->getMainCanvas();
    StyledMultiShape2D *gui = c->getGui();

    MultiText *guiText = c->addLayer(new MultiText(c, s));
    
    testGapMinder(gui, guiText);
    
  }
};

int main(int argc, char *argv[]) {
  
  return GLWin::init(new TestWidgets(), 1024, 600);

}

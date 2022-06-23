#include <cstdlib>
#include <string>
#include <vector>

#include "data/GapMinderLoader.hh"
#include "opengl/GapMinderWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/GraphStyle.hh"

using namespace std;
using namespace grail;

class TestGapMinderWidget : public GraphStyle {
 private:
  GapMinderWidget* chart;

 public:
  TestGapMinderWidget(Tab* tab) : GraphStyle(tab, "TIMES", 24, 12) {
    MainCanvas* c = tab->getMainCanvas();
    const Style* s =
        new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text
    StyledMultiShape2D* gui = c->getGui();
    MultiText* guiText = c->addLayer(new MultiText(c, s));
    string xData = "gdp_per_capita.csv";
    string yData = "ddf--datapoints--poisonings_deaths_per_100000_people.csv";
    string sData = "ddf--datapoints--vacc_rate--by--country--time.csv";

    vector<float> x2 = {65000};
    vector<float> y2 = {14};
    chart = new GapMinderWidget(gui, guiText, 75, 50, 900, 450, x2, y2);
    chart->loadData(yData, xData, sData, 2000, 2010);

    chart->setTitle("GDP per Capita vs. Poisonings per 100k");
    chart->init();
  }

  ~TestGapMinderWidget() { delete chart; }
  TestGapMinderWidget(const TestGapMinderWidget& orig) = delete;
  TestGapMinderWidget& operator=(const TestGapMinderWidget& orig) = delete;

  void update() { chart->animate(10000, 2); }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* tab) {
  w->setTitle("Test Gap Minder Widget");
  tab->addMember(new TestGapMinderWidget(tab));
}

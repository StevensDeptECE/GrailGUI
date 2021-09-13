#include <string>
#include <vector>
#include <cstdlib>
#include "opengl/GraphStyle.hh"
#include "opengl/GapMinderWidget.hh"
#include "data/GapMinderLoader.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;


class TestGapMinderWidget : public Animated {
 private:
  GapMinderWidget* chart;
 public:
  
  TestGapMinderWidget(Tab* tab, const GraphStyle* gs) : Animated(tab, "Test GapMinder Graph", 1000, 600) {
    MainCanvas *c = tab->getMainCanvas();
    const Style *s = new Style("TIMES", 24, 1, 0, 0, 0,  // black background (unused)
                  0, 0, 0);                 // black foreground text
    StyledMultiShape2D *gui = c->getGui();
    MultiText *guiText = c->addLayer(new MultiText(c, s));
    string xData = "gdp_per_capita.csv";
    string yData = "ddf--datapoints--poisonings_deaths_per_100000_people.csv";
    string sData = "ddf--datapoints--vacc_rate--by--country--time.csv";

    vector <float> x2 = {65000};
    vector <float> y2 = {14};
    chart = new GapMinderWidget(c, 75, 50, 900, 450,
																GraphWidget::AxisType::LINEAR, GraphWidget::AxisType::LINEAR,
																GraphWidget::AxisType::LINEAR, gs);
    chart->loadData(yData, xData, sData, 2000, 2010);

    chart->setTitle("GDP per Capita vs. Poisonings per 100k");
    chart->init();
  }

  ~TestGapMinderWidget() { delete chart; }
  TestGapMinderWidget(const TestGapMinderWidget& orig) = delete;
  TestGapMinderWidget& operator =(const TestGapMinderWidget& orig) = delete;

  void update(){
		chart->update();
  }
};

void grailmain(int argc, char *argv[], GLWin* w, Tab* tab) {
  new TestGapMinderWidget(tab, GraphStyle::steelblue);
}

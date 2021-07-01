#include "opengl/GrailGUI.hh"
#include "opengl/LineGraphWidget.hh"

using namespace std;
using namespace grail;

class TestLineGraph : public GLWin {
 public:
  TestLineGraph() : GLWin(0x000000, 0xCCCCCC, "TestLineGraph") {}

  void init() {
    const Style *lineGraphTitleStyle =
        new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);

    const Style *xAxisTitleStyle =
        new Style("TIMES", 12, 1, 0, 0, 0, 1, 0, 0);  // should be red

    // const Style *xAxisLabelStyle =
    //     new Style("TIMES", 12, 1, 0, 0, 0, 0, 1, 0);  // should be green

    const Style *yAxisTitleStyle =
        new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 1);  // should be blue

    // const Style *yAxisLabelStyle =
    //     new Style("TIMES", 12, 1, 0, 0, 0, 1, 0, 1);  // should be purple

    MainCanvas *c = currentTab()->getMainCanvas();

    StyledMultiShape2D *m = c->getGui();

    MultiText *lineGraphTitleText =
        c->addLayer(new MultiText(c, lineGraphTitleStyle));

    vector<double> times = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<double> values = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    vector<double> logValues = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    LineGraphWidget lgw(c, m, lineGraphTitleText, 100, 100, 800, 800);
    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    lgw.setGraphTitle("Test Title");
    lgw.setXAxisTextStyle(xAxisTitleStyle);
    lgw.setYAxisTextStyle(yAxisTitleStyle);
    lgw.setPointFormat('t', 5, grail::red);

    // x axis stuff
    lgw.setXPoints(times);
    lgw.createXAxis(GraphWidget::AxisType::LINEAR);

    // set x axis parameters
    lgw.xAxis->setBounds(times.front(), times.back());
    lgw.xAxis->setTickInterval(2);

    lgw.xAxis->setTickDrawSize(10);
    lgw.xAxis->setShowTicks(true);
    lgw.xAxis->setTitle("x axis");
    lgw.xAxis->setAxisColor(grail::green);
    lgw.xAxis->setTickColor(grail::purple);
    lgw.xAxis->setTickFormat(3, 1);

    // y axis stuff
    lgw.createYAxis(GraphWidget::AxisType::LINEAR);
    lgw.setYPoints(values);
    // lgw.createYAxis(GraphWidget::AxisType::LOGARITHMIC);
    // lgw.setYPoints(logValues);

    // set y axis parameters

    lgw.yAxis->setBounds(values.front(), values.back());
    // lgw.yAxis->setBounds(logValues.front(), logValues.back());
    lgw.yAxis->setIsVert(true);
    // linear
    // lgw.yAxis->setTickInterval(3);
    // logarithmic
    lgw.yAxis->setTickInterval(2);
    lgw.yAxis->setTickFormat(4, 1);

    lgw.yAxis->setTickDrawSize(8);
    lgw.yAxis->setShowTicks(true);
    lgw.yAxis->setTitle("y axis");
    lgw.yAxis->setAxisColor(grail::yellow);
    lgw.yAxis->setTickColor(grail::red);

    lgw.init();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestLineGraph(), 1000, 1000);
}

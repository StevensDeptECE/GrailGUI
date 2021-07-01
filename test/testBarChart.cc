#include "opengl/BarChartWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestBarChart : public GLWin {
 public:
  TestBarChart() : GLWin(0x000000, 0xCCCCCC, "TestBarChart") {}

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

    MultiText *barChartTitleText =
        c->addLayer(new MultiText(c, lineGraphTitleStyle));

    vector<double> values = {4, 6, 8, 10, 12, 14};
    vector<double> logValues = {4, 8, 16, 32, 64, 128};

    vector<string> names = {"red",   "orange", "yellow",
                            "green", "blue",   "purple"};
    vector<glm::vec4> colors = {grail::red,   grail::darkred, grail::yellow,
                                grail::green, grail::blue,    grail::purple};

    BarChartWidget bcw(c, m, barChartTitleText, 200, 200, 600, 600);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    bcw.setGraphTitle("Test Title");
    bcw.setBarWidth(45);
    bcw.setBarColors(colors);
    bcw.setXAxisTextStyle(xAxisTitleStyle);
    bcw.setYAxisTextStyle(yAxisTitleStyle);

    // create x axis (categories)
    bcw.setNames(names);
    bcw.createXAxis();

    // set releavant x axis parameters
    // if you try to set something not applicable to a text axis (as that's what
    // the x axis always will be), the compiler will yell at you
    bcw.xAxis->setTitle("Colors");
    bcw.xAxis->setTickDrawSize(7);
    bcw.xAxis->setAxisColor(grail::green);
    bcw.xAxis->setTickColor(grail::purple);

    // y axis stuff
    bcw.createYAxis(GraphWidget::AxisType::LINEAR);
    bcw.setValues(values);
    // bcw.createYAxis(GraphWidget::AxisType::LOGARITHMIC);
    // bcw.setValues(logValues);

    // set y axis parameters
    bcw.yAxis->setIsVert(true);

    // linear
    bcw.yAxis->setBounds(2, 20);
    bcw.yAxis->setTickInterval(2);

    // logarithmic
    // bcw.yAxis->setBounds(1, 256);
    // bcw.yAxis->setTickInterval(2);
    // bcw.yAxis->setTickFormat(4, 1);

    bcw.yAxis->setTickDrawSize(8);
    bcw.yAxis->setShowTicks(true);
    bcw.yAxis->setTitle("y axis");
    bcw.yAxis->setAxisColor(grail::yellow);
    bcw.yAxis->setTickColor(grail::red);

    bcw.init();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestBarChart(), 1000, 1000);
}

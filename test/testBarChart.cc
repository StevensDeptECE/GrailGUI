#include "opengl/BarChartWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestBarChart : public GLWin {
 private:
  const Style *baseGraphStyle;
  const Style *xAxisStyle;
  const Style *xAxisTextStyle;
  const Style *yAxisStyle;
  const Style *yAxisTextStyle;
  const Style *barStyle;

 public:
  TestBarChart() : GLWin(0x000000, 0xCCCCCC, "TestBarChart") {}
  ~TestBarChart() {
    delete baseGraphStyle;
    delete xAxisStyle;
    delete xAxisTextStyle;
    delete yAxisStyle;
    delete yAxisTextStyle;
    delete barStyle;
  }

  void init() {
    // two lines and the overall title
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0, 5);

    // will control how thick lines for x axis are drawn
    xAxisStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 1, 0, 4);

    // controls the font, size, and color of x axis text
    xAxisTextStyle = new Style("TIMES", 12, 1, 0, 0, 0, 1, 0, 0, 3);

    // will control how thick lines for y axis are drawn
    yAxisStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 1, 2);

    // controls the font, size, and color of y axis text
    yAxisTextStyle = new Style("TIMES", 12, 1, 0, 0, 0, 1, 0, 1, 1);

    // controls the thickness of lines drawn by LineGraphWidget
    barStyle = new Style("TIMES", 12, 1, 0, 0, 0, 1, 0, 1, 3);

    MainCanvas *c = currentTab()->getMainCanvas();

    vector<double> values = {4, 6, 8, 10, 12, 14};
    vector<double> logValues = {4, 8, 16, 32, 64, 128};

    vector<string> names = {"red",   "orange", "yellow",
                            "green", "blue",   "purple"};

    vector<glm::vec4> colors = {grail::red,   grail::orange, grail::yellow,
                                grail::green, grail::blue,   grail::purple};

    vector<glm::vec4> outline{grail::green, grail::blue, grail::purple};

    BarChartWidget bcw(c, 200, 200, 400, 400);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    bcw.setGraphTitle("Test Title");
    bcw.setBaseStyle(baseGraphStyle);
    bcw.setXAxisStyle(xAxisStyle);
    bcw.setYAxisStyle(yAxisStyle);
    bcw.setXAxisTextStyle(xAxisTextStyle);
    bcw.setYAxisTextStyle(yAxisTextStyle);
    bcw.setBarStyle(barStyle);

    // bar chart widget specific bits
    bcw.setBarColors(colors);
    bcw.setBarOutlineColors(outline);

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
    bcw.yAxis->setBounds(0, 60);
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

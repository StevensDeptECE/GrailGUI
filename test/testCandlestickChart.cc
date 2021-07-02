#include "opengl/CandlestickChartWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestBoxChart : public GLWin {
 private:
  const Style *baseGraphStyle;
  const Style *xAxisStyle;
  const Style *xAxisTextStyle;
  const Style *yAxisStyle;
  const Style *yAxisTextStyle;
  const Style *dataStyle;

 public:
  TestBoxChart() : GLWin(0x000000, 0xCCCCCC, "TestBarChart") {}
  ~TestBoxChart() {
    delete baseGraphStyle;
    delete xAxisStyle;
    delete xAxisTextStyle;
    delete yAxisStyle;
    delete yAxisTextStyle;
    delete dataStyle;
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
    dataStyle = new Style("TIMES", 12, 1, 0, 0, 0, 1, 0, 1, 3);
    MainCanvas *c = currentTab()->getMainCanvas();

    vector<double> data = {
        150, 152, 153, 154,  //
        153, 154, 155, 158,  //
        155, 156, 157, 162   //
    };

    vector<string> names = {"one", "two", "seven", "three"};
    vector<glm::vec4> boxColors = {grail::red, grail::green, grail::blue};
    vector<glm::vec4> whiskerColors = {grail::cyan, grail::purple};
    vector<glm::vec4> outlineColors = {grail::darkblue, grail::darkgreen};

    CandlestickChartWidget ccw(c, 100, 100, 400, 400);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    ccw.setGraphTitle("Test Title");
    ccw.setBaseStyle(baseGraphStyle);
    ccw.setXAxisStyle(xAxisStyle);
    ccw.setYAxisStyle(yAxisStyle);
    ccw.setXAxisTextStyle(xAxisTextStyle);
    ccw.setYAxisTextStyle(yAxisTextStyle);
    ccw.setDataStyle(dataStyle);

    // bar chart widget specific bits
    ccw.setBoxWidth(45);
    ccw.setBoxColors(boxColors);
    ccw.setBoxOutlineColors(outlineColors);

    // create x axis (categories)
    ccw.setNames(names);
    ccw.createXAxis();

    // set releavant x axis parameters
    // if you try to set something not applicable to a text axis (as that's what
    // the x axis always will be), the compiler will yell at you
    ccw.xAxis->setTitle("Colors");
    ccw.xAxis->setTickDrawSize(7);
    ccw.xAxis->setAxisColor(grail::green);
    ccw.xAxis->setTickColor(grail::purple);

    // y axis stuff
    ccw.createYAxis(GraphWidget::AxisType::LINEAR);
    ccw.setData(data);

    // set y axis parameters
    ccw.yAxis->setIsVert(true);

    // linear
    ccw.yAxis->setBounds(148, 165);
    ccw.yAxis->setTickInterval(2);

    ccw.yAxis->setTickDrawSize(8);
    ccw.yAxis->setShowTicks(true);
    ccw.yAxis->setTitle("y axis");
    ccw.yAxis->setAxisColor(grail::yellow);
    ccw.yAxis->setTickColor(grail::red);

    ccw.init();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestBoxChart(), 1000, 1000);
}

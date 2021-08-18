#include "opengl/GraphStyle.hh"
#include "opengl/BoxChartWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestBoxChart : public GraphStyle {
 public:
  TestBoxChart(Tab* tab) : GraphStyle(tab, "TIMES", 20, 12) {
    MainCanvas *c = tab->getMainCanvas();

    vector<double> data = {150, 350, 222, 100, 300,  //
                           130, 300, 250, 190, 170,  //
                           100, 50,  20,  150, 200,  //
                           330, 200, 270, 180, 300,  //
                           49,  247, 325, 114, 89};

    vector<string> names = {"red", "orange", "yellow", "green", "blue"};
    vector<glm::vec4> boxColors = {grail::red, grail::green, grail::blue};
    vector<glm::vec4> whiskerColors = {grail::cyan, grail::purple};
    vector<glm::vec4> outlineColors = {grail::darkblue, grail::darkgreen};

    BoxChartWidget bcw(c, 300, 300, 400, 200);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    bcw.setGraphTitle("Test Title");
		bcw.setStyle(this);
#if 0
    bcw.setBaseStyle(baseGraphStyle);
    bcw.setXAxisStyle(xAxisStyle);
    bcw.setYAxisStyle(yAxisStyle);
    bcw.setXAxisTextStyle(xAxisTextStyle);
    bcw.setYAxisTextStyle(yAxisTextStyle);
    bcw.setWhiskerStyle(whiskerStyle);
    bcw.setBoxStyle(boxStyle);
#endif
    // bar chart widget specific bits
    bcw.setBoxWidth(45);
    bcw.setPointsPerBox(5);
    bcw.setBoxColors(boxColors);
    bcw.setWhiskerColors(whiskerColors);
    bcw.setOutlineColors(outlineColors);

    // create x axis (categories)
    bcw.setNames(names);
    bcw.createXAxis(GraphWidget::AxisType::TEXT);

    // set releavant x axis parameters
    // if you try to set something not applicable to a text axis (as that's what
    // the x axis always will be), the compiler will yell at you
    bcw.xAxis->setTitle("Colors");
    bcw.xAxis->setTickDrawSize(7);
    bcw.xAxis->setAxisColor(grail::green);
    bcw.xAxis->setTickColor(grail::purple);

    // y axis stuff
    bcw.createYAxis(GraphWidget::AxisType::LINEAR);
    bcw.setData(data);

    // set y axis parameters
    bcw.yAxis->setIsVert(true);

    // linear
    bcw.yAxis->setBounds(0, 500);
    bcw.yAxis->setTickInterval(75);

    bcw.yAxis->setTickDrawSize(8);
    bcw.yAxis->setShowTicks(true);
    bcw.yAxis->setTitle("y axis");
    bcw.yAxis->setAxisColor(grail::yellow);
    bcw.yAxis->setTickColor(grail::red);

    bcw.init();
  }
};

void grailmain(int argc, char *argv[], GLWin* w, Tab* tab) {
	w->setTitle("Test Bar Chart");
  tab->addAnimated(new TestBoxChart(tab));
}

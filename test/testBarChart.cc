#include "opengl/GraphStyle.hh"
#include "opengl/BarChartWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestBarChart : public Animated {
public:
	TestBarChart(Tab* tab, const GraphStyle* gs) : Animated(tab) {
    MainCanvas *c = tab->getMainCanvas();
    vector<double> values = {4, 6, 8, 10, 12, 14};
    vector<double> logValues = {4, 8, 16, 32, 64, 128};

    vector<string> names = {"red",   "orange", "yellow",
                            "green", "blue",   "purple"};

    vector<glm::vec4> colors = {grail::red,   grail::orange, grail::yellow,
                                grail::green, grail::blue,   grail::purple};

    vector<glm::vec4> outline{grail::green, grail::blue, grail::purple};

    BarChartWidget bcw(c, 100, 100, 850, 400, GraphWidget::AxisType::TEXT, GraphWidget::AxisType::LINEAR, gs);

    // create x axis (categories)
    bcw.setNames(names);
    //    bcw.createXAxis(GraphWidget::AxisType::TEXT);
    bcw.xAxis->setTitle("Colors");

    // y axis stuff
    //bcw.createYAxis(GraphWidget::AxisType::LINEAR);
    bcw.setValues(values);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    bcw.setTitle("Test Title");
		//bcw.setStyle(this);

    // bar chart widget specific bits
    //bcw.setBarColors(colors);
    //bcw.setBarOutlineColors(outline);
    // linear
    bcw.yAxis->setBounds(0, 60);
    bcw.yAxis->setTickInterval(2);
    bcw.yAxis->setTitle("y axis");

    // logarithmic
    // bcw.createYAxis(GraphWidget::AxisType::LOGARITHMIC);
    // bcw.yAxis->setBounds(1, 256);
    // bcw.yAxis->setTickInterval(2);
    // bcw.yAxis->setTickFormat(4, 1);
    // bcw.setValues(logValues);

    bcw.init();
  }
};

void grailmain(int argc, char *argv[], GLWin* w, Tab* tab) {
  new TestBarChart(tab, GraphStyle::steelblue);
}

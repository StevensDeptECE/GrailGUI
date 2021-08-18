#include "opengl/GraphStyle.hh"
#include "opengl/BarChartWidget.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestBarChart : public GraphStyle {
public:
	constexpr static const char faceName[] = "TIMES";
	TestBarChart(Tab* tab) : GraphStyle(tab) {
    MainCanvas *c = tab->getMainCanvas();
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
		bcw.setStyle(this);

    // bar chart widget specific bits
    bcw.setBarColors(colors);
    bcw.setBarOutlineColors(outline);

    // create x axis (categories)
    bcw.setNames(names);
    bcw.createXAxis(GraphWidget::AxisType::TEXT);

    // set releavant x axis parameters
    // if you try to set something not applicable to a text axis (as that's what
    // the x axis always will be), the compiler will yell at you
    bcw.xAxis->setTitle("Colors");
    bcw.xAxis->setTickDrawSize(7);
    bcw.xAxis->setAxisColor(grail::green);
    bcw.xAxis->setTickColor(grail::darkgreen);

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

void grailmain(int argc, char *argv[], GLWin* w, Tab* tab) {
	w->setTitle("Test Bar Chart");
  tab->addAnimated(new TestBarChart(tab));
}

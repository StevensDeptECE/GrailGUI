#include "opengl/BarChartWidget.hh"
#include "opengl/BoxChartWidget.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/GraphStyle.hh"
#include "opengl/LineGraphWidget.hh"
#include "opengl/NavigationBar.hh"

using namespace std;
using namespace grail;

class BarChart : public GraphStyle {
 public:
  BarChart(GLWin* w) : GraphStyle(w->addTab(), "TIMES", 20, 12) {
    MainCanvas* c = tab->getMainCanvas();
    vector<double> values = {4, 6, 8, 10, 12, 14};
    vector<double> logValues = {4, 8, 16, 32, 64, 128};

    vector<string> names = {"red",   "orange", "yellow",
                            "green", "blue",   "purple"};

    vector<glm::vec4> colors = {grail::red,   grail::orange, grail::yellow,
                                grail::green, grail::blue,   grail::purple};

    vector<glm::vec4> outline{grail::green, grail::blue, grail::purple};

    BarChartWidget bcw(c, 100, 100, 850, 400, GraphWidget::AxisType::TEXT,
                       GraphWidget::AxisType::LINEAR, this);

    // create x axis (categories)
    bcw.setNames(names);
    //    bcw.createXAxis(GraphWidget::AxisType::TEXT);
    bcw.xAxis->setTitle("Colors");

    // y axis stuff
    // bcw.createYAxis(GraphWidget::AxisType::LINEAR);
    bcw.setValues(values);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    bcw.setGraphTitle("Bar Chart");
    // bcw.setStyle(this);

    // bar chart widget specific bits
    // bcw.setBarColors(colors);
    // bcw.setBarOutlineColors(outline);
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

class BoxChart : public GraphStyle {
 public:
  BoxChart(GLWin* w) : GraphStyle(w->addTab(), "TIMES", 20, 12) {
    MainCanvas* c = tab->getMainCanvas();

    vector<double> data = {150, 350, 222, 100, 300,  //
                           130, 300, 250, 190, 170,  //
                           100, 50,  20,  150, 200,  //
                           330, 200, 270, 180, 300,  //
                           49,  247, 325, 114, 89};

    vector<string> names = {"red", "orange", "yellow", "green", "blue"};

    BoxChartWidget bcw(c, 100, 300, 850, 200, GraphWidget::AxisType::TEXT,
                       GraphWidget::AxisType::LINEAR, this);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    bcw.setGraphTitle("Box Chart");

    // create x axis (categories)
    bcw.setNames(names);

    // set releavant x axis parameters
    // if you try to set something not applicable to a text axis (as that's what
    // the x axis always will be), the compiler will yell at you
    bcw.xAxis->setTitle("Colors");

    // y axis stuff
    bcw.setData(data);
    bcw.yAxis->setTitle("y axis");
    // linear
    bcw.yAxis->setBounds(0, 500);
    bcw.yAxis->setTickInterval(75);

    // set y axis parameters
    // bar chart widget specific bits
    bcw.setBoxWidth(45);
    bcw.setPointsPerBox(5);

    bcw.init();
  }
};

class LineGraph : public GraphStyle {
 public:
  LineGraph(GLWin* w) : GraphStyle(w->addTab(), "TIMES", 24, 12) {
    MainCanvas* c = tab->getMainCanvas();

    vector<double> times = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<double> values = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    vector<double> logValues = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    LineGraphWidget lgw(c, 250, 200, 500, 300, GraphWidget::AxisType::LINEAR,
                        GraphWidget::AxisType::LOGARITHMIC, this);

    // Graph Superclass settings
    lgw.setGraphTitle("Line Graph");

    // LineGraphWidget specific settings
    lgw.setPointFormat('t', 5, grail::red);

    // x axis stuff
    lgw.setXPoints(times);

    // set x axis parameters
    lgw.xAxis->setBounds(times.front(), times.back());
    lgw.xAxis->setTickInterval(2);
    lgw.xAxis->setTitle("x axis");
    lgw.xAxis->setTickFormat(3, 1);

    // y axis stuff
    lgw.setYPoints(logValues);

    // set y axis parameters
    lgw.yAxis->setBounds(logValues.front(), logValues.back());
    lgw.yAxis->setTickInterval(2);
    lgw.yAxis->setTickFormat(4, 1);
    lgw.yAxis->setTitle("y axis");

    lgw.init();
  }
};

class Menu : public NavigationBar {
 public:
  Menu(GLWin* w);
};

Menu::Menu(GLWin* w) : NavigationBar(w, 150, 750, 300, 50, 40) {
  ButtonWidget* bar = addButton(100, 50, "Bar", "tab 1");
  bar->setAction([w]() { w->switchTab(2); });

  ButtonWidget* box = addButton(100, 50, "Box", "tab 2");
  box->setAction([w]() { w->switchTab(1); });

  ButtonWidget* line = addButton(100, 50, "Line", "tab 3");
  line->setAction([w]() { w->switchTab(0); });
}

void grailmain(int argc, char* argv[], GLWin* w) {
  new LineGraph(w);
  new BoxChart(w);
  new Menu(w);
  new BarChart(w);
}
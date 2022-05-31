#include "opengl/BarChartWidget.hh"
#include "opengl/BoxChartWidget.hh"
#include "opengl/CandlestickChartWidget.hh"
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

class CandlestickChart : public GraphStyle {
 public:
  CandlestickChart(GLWin* w) : GraphStyle(w->addTab(), "TIMES", 20, 12) {
    MainCanvas* c = tab->getMainCanvas();

    vector<double> data = {
        153.25, 154.16, 152.99, 153.68, 153.34, 153.73, 152.15, 152.73, 152.66,
        153.58, 151.46, 153.57, 153.71, 153.9,  152.17, 153.26, 153.46, 154.53,
        153.36, 153.92, 153.08, 153.31, 151.91, 152.93, 153.63, 154.88, 153.5,
        154.83, 154.41, 154.93, 154.01, 154.74, 155.25, 156.17, 154.93, 155.28,
        155.5,  157.43, 154.54, 155.34, 155.41, 156.42, 155.19, 155.99, 155.79,
        156.81, 155.17, 155.44, 156.17, 156.72, 155.85, 156.43, 157.33, 159.43,
        157.09, 159.31, 159.21, 160.29, 158.66, 159.9,  159.85, 160.13, 159.03,
        159.19, 156.19, 156.52, 154.46, 155.42, 156.05, 157.18, 155.4,  155.69,
        156.33, 157.13, 154.94, 155.61, 155.73, 156.86, 155.64, 156.54, 156.35,
        156.99, 154.71, 155.85, 156.67, 157.26, 156.22, 156.85, 158.72, 163.01,
        158.13, 162.47, 163.3,  167.47, 163.13, 166.12, 167.3,  169.04, 166.34,
        168.43, 169.26, 169.33, 165.02, 166.29, 167.04, 167.9,  164.69, 167.51,
        173.38, 173.64, 170.51, 171.88, 171.75, 174.36, 171.1,  173.63, 173.29,
        174.51, 173.29, 174.18, 174.03, 175.61, 173.71, 175.61, 174.48, 175.46,
        172.52, 175.25, 175.11, 175.38, 174.27, 174.67};
    vector<string> names = {"one", "two", "seven", "three"};
    CandlestickChartWidget ccw(c, 100, 100, 850, 400,
                               GraphWidget::AxisType::LINEAR,
                               GraphWidget::AxisType::LINEAR, this);

    // setting general things for the graph
    // the axis text styles must be set before
    // creating the axes
    ccw.setGraphTitle("Candlestick Chart");

    // bar chart widget specific bits
    // ccw.setBoxWidth(5);
    // ccw.setBoxColors(boxColors);
    // ccw.setBoxOutlineColors(outlineColors);

    // create x axis (categories)
    // ccw.setNames(names);

    // set relevant x axis parameters
    // if you try to set something not applicable to a text axis (as that's what
    // the x axis always will be), the compiler will yell at you
    ccw.xAxis->setTitle("Date");
    //    ccw.xAxis->setTickDrawSize(7);
    // ccw.xAxis->setAxisColor(grail::green);
    // ccw.xAxis->setTickColor(grail::purple);

    // y axis stuff
    ccw.setData(data);

    // set y axis parameters
    // ccw.yAxis->setIsVert(true);

    // linear
    ccw.yAxis->setBounds(140, 180);
    ccw.yAxis->setTickInterval(2);

    // ccw.yAxis->setTickDrawSize(10);
    // ccw.yAxis->setShowTicks(true);
    ccw.yAxis->setTitle("Price");
    // ccw.yAxis->setAxisColor(grail::yellow);
    // ccw.yAxis->setTickColor(grail::red);

    ccw.init();
  }
};

class Menu : public NavigationBar {
 public:
  Menu(GLWin* w);
};

Menu::Menu(GLWin* w) : NavigationBar(w, 40, 600, 300, 50, 40) {
  ButtonWidget* bar = addButton(175, 50, "Bar", "tab 1");
  bar->setAction([w]() { w->switchTab(3); });

  ButtonWidget* box = addButton(175, 50, "Box", "tab 2");
  box->setAction([w]() { w->switchTab(2); });

  ButtonWidget* line = addButton(175, 50, "Line", "tab 3");
  line->setAction([w]() { w->switchTab(1); });

  ButtonWidget* cstick = addButton(175, 50, "CStick", "tab 4");
  cstick->setAction([w]() { w->switchTab(0); });
}

void grailmain(int argc, char* argv[], GLWin* w) {
  new CandlestickChart(w);
  new LineGraph(w);
  new BoxChart(w);
  new BarChart(w);
  new Menu(w);
}
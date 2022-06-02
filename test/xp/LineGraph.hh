#pragma once
#include "opengl/LineGraphWidget.hh"

using namespace std;

class LineGraph : public GraphStyle {
 public:
  LineGraph(GLWin* w) : GraphStyle(w->addTab(), "TIMES", 24, 12) {
    MainCanvas* c = tab->getMainCanvas();

    vector<double> times = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<double> values = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};
    vector<double> logValues = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

    LineGraphWidget lgw(c, 250, 300, 500, 300, GraphWidget::AxisType::LINEAR,
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
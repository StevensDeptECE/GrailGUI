#pragma once

#include "opengl/GraphWidget.hh"

class BarChartWidget : public GraphWidget {
 private:
  std::vector<std::string> names;
  std::vector<double> values;
  float barWidth;

 public:
  BarChartWidget(Canvas *c, double x, double y, double w, double h, AxisType xAxisType, AxisType yAxisType, const GraphStyle* s)
      : GraphWidget(c, x, y, w, h, DISCRETE_PERMITTED, FUNCTIONS_PERMITTED, xAxisType, yAxisType, s),
        values(std::vector<double>()),
        names(std::vector<std::string>()),
        barWidth(20) {}

  void setBarWidth(double width);
  void setValues(const std::vector<double> &values);
  void setNames(const std::vector<std::string> &names);
  void init() override;
};

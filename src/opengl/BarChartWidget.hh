#pragma once

#include "opengl/GraphWidget.hh"

class BarChartWidget : public GraphWidget {
 private:
  std::vector<std::string> names;
  std::vector<double> values;
  glm::vec4 &barColor;
  double barWidth;

 public:
  TextAxisWidget *xAxis;
  BarChartWidget(Canvas *c, StyledMultiShape2D *m, MultiText *t, double x,
                 double y, double w, double h)
      : GraphWidget(c, m, t, x, y, w, h),
        values(std::vector<double>()),
        names(std::vector<std::string>()),
        barColor(grail::blue),
        barWidth(20) {}

  void setBarWidth(double width);
  void setBarColor(glm::vec4 &color);
  void setValues(const std::vector<double> &values);
  void setNames(const std::vector<std::string> &names);
  void createXAxis(AxisType a = AxisType::TEXT) override;
  void createYAxis(AxisType a) override;
  void init() override;
};

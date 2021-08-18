#pragma once

#include "opengl/GraphWidget.hh"

class CandlestickChartWidget : public GraphWidget {
 private:
  std::vector<std::string> names;
  std::vector<double> data;
  float boxWidth;

 public:
  CandlestickChartWidget(Canvas* c, double x, double y, double w, double h, AxisType xAxisType, AxisType yAxisType, const GraphStyle* s)
      : GraphWidget(c, x, y, w, h, FUNCTIONS_PERMITTED, FUNCTIONS_PERMITTED, xAxisType, yAxisType, s),
        data(std::vector<double>()),
        boxWidth(3) {}
  void setBoxWidth(double width);
  void setData(const std::vector<double>& data);
  void setNames(const std::vector<std::string>& names);
  void init() override;
};

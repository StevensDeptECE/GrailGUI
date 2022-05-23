#pragma once

#include "opengl/GraphWidget.hh"

class BoxChartWidget : public GraphWidget {
 private:
  // TODO: remove this style and add the necessary number of styles to have
  // variable thicknesses for the min and max lines, possibly the center lines
  // connecting them to the main box, and the thickness of the outline of the
  // main box
  std::vector<double> data;
  std::vector<std::string> names;

  int pointsPerBox;
  double boxWidth;

 public:
  // c, 300, 300, 400, 200, GraphWidget::AxisType::TEXT,
  // GraphWidget::AxisType::LINEAR, this);
  BoxChartWidget(Canvas* c, float x, float y, float w, float h,
                 AxisType xAxisType, AxisType yAxisType, const GraphStyle* s)
      : GraphWidget(c, x, y, w, h, DISCRETE_PERMITTED, FUNCTIONS_PERMITTED,
                    xAxisType, yAxisType, s),
        data(std::vector<double>()),
        names(std::vector<std::string>()),
        boxWidth(20),
        pointsPerBox(3) {
    setStyle(s);
  }

  void setBoxWidth(double width);
  void setPointsPerBox(int n);
  void setData(const std::vector<double>& data);
  void setNames(const std::vector<std::string>& names);
  void init() override;
};

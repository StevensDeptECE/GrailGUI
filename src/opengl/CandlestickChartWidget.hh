#pragma once

#include "opengl/GraphWidget.hh"

class CandlestickChartWidget : public GraphWidget {
 private:
  const Style* lineStyle;
  const Style* boxStyle;
  std::vector<std::string> names;
  std::vector<double> data;
  std::vector<glm::vec4> boxColors;
  std::vector<glm::vec4> outlineColors;

  double boxWidth;

 public:
  CandlestickChartWidget(Canvas* c, double x, double y, double w, double h)
      : GraphWidget(c, x, y, w, h, DISCRETE_PERMITTED, FUNCTIONS_PERMITTED),
        lineStyle(nullptr),
        boxStyle(nullptr),
        data(std::vector<double>()),
        boxColors({grail::blue}),
        outlineColors({grail::black}),
        boxWidth(3) {}

  void setLineStyle(const Style* s);
  void setBoxStyle(const Style* s);
  void setBoxWidth(double width);
  void setBoxColors(const std::vector<glm::vec4>& colors);
  void setBoxOutlineColors(const std::vector<glm::vec4>& colors);
  void setData(const std::vector<double>& data);
  void setNames(const std::vector<std::string>& names);
  void init() override;
};

#pragma once

#include "opengl/GraphWidget.hh"

class BarChartWidget : public GraphWidget {
 private:
  const Style *barStyle;  // for the thickness of bar outlines
  std::vector<std::string> names;
  std::vector<double> values;
  std::vector<glm::vec4> barColors;
  std::vector<glm::vec4> barOutlineColors;
  double barWidth;

 public:
  BarChartWidget(Canvas *c, double x, double y, double w, double h)
      : GraphWidget(c, x, y, w, h, DISCRETE_PERMITTED, FUNCTIONS_PERMITTED),
        barStyle(nullptr),
        values(std::vector<double>()),
        names(std::vector<std::string>()),
        barColors({grail::blue}),
        barOutlineColors({grail::blue}),
        barWidth(20) {}

	void setStyle(const GraphStyle* gs);
  void setBarStyle(const Style *s);
  void setBarWidth(double width);
  void setBarColors(const std::vector<glm::vec4> &colors);
  void setBarOutlineColors(const std::vector<glm::vec4> &colors);
  void setValues(const std::vector<double> &values);
  void setNames(const std::vector<std::string> &names);
  void init() override;
};

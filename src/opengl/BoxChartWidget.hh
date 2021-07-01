#pragma once

#include "opengl/GraphWidget.hh"

class BoxChartWidget : public GraphWidget {
 private:
  std::vector<double> data;
  std::vector<std::string> names;
  std::vector<glm::vec4> boxColors;
  std::vector<glm::vec4> whiskerColors;
  std::vector<glm::vec4> outlineColors;

  double boxWidth;
  // int outlineThickness;
  // int whiskerThickness;

  int pointsPerBox;

 public:
  BoxChartWidget(Canvas *c, StyledMultiShape2D *m, MultiText *t, double x,
                 double y, double w, double h)
      : GraphWidget(c, m, t, x, y, w, h),
        data(std::vector<double>()),
        names(std::vector<std::string>()),
        boxColors({grail::blue}),
        whiskerColors({grail::black}),
        outlineColors({grail::black}),
        boxWidth(20),
        // outlineThickness(1),
        // whiskerThickness(1),
        pointsPerBox(3) {}

  void setBoxWidth(double width);
  void setBoxColors(std::vector<glm::vec4> &colors);
  void setWhiskerColors(std::vector<glm::vec4> &colors);
  void setOutlineColors(std::vector<glm::vec4> &colors);
  // void setOutlineThickness(int n);
  // void setWhiskerThickness(int n);
  void setPointsPerBox(int n);
  void setData(const std::vector<double> &data);
  void setNames(const std::vector<std::string> &names);
  void createXAxis(AxisType a = AxisType::TEXT) override;
  void createYAxis(AxisType a = AxisType::LINEAR) override;
  void init() override;
};

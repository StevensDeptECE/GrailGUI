#pragma once

#include "opengl/GraphWidget.hh"

class BoxChartWidget : public GraphWidget {
 private:
  std::vector<double> data;
  std::vector<std::string> names;
  glm::vec4 &boxColor;
  double boxWidth;
  int pointsPerBox;

 public:
  TextAxisWidget *xAxis;
  BoxChartWidget(Canvas *c, StyledMultiShape2D *m, MultiText *t, double x,
                 double y, double w, double h)
      : GraphWidget(c, m, t, x, y, w, h),
        data(std::vector<double>()),
        names(std::vector<std::string>()),
        boxColor(grail::blue),
        boxWidth(20),
        pointsPerBox(3) {}

  void setBoxWidth(double width);
  void setBoxColor(glm::vec4 &color);
  void setPointsPerBox(int n);
  void setData(const std::vector<double> &data);
  void setNames(const std::vector<std::string> &names);
  void createXAxis(AxisType a = AxisType::TEXT) override;
  void createYAxis(AxisType a = AxisType::LINEAR) override;

  // void chart(const std::vector<float>& b, int rulerInterval,
  //            int dataPointsPerBox, const std::vector<std::string>& boxNames);
  void init() override;
};

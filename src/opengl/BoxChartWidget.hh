#pragma once

#include "opengl/GraphWidget.hh"

class BoxChartWidget : public GraphWidget {
 private:
  // TODO: remove this style and add the necessary number of styles to have
  // variable thicknesses for the min and max lines, possibly the center lines
  // connecting them to the main box, and the thickness of the outline of the
  // main box
  const Style* whiskerStyle;
  const Style* boxStyle;
  std::vector<double> data;
  std::vector<std::string> names;
  std::vector<glm::vec4> boxColors;
  std::vector<glm::vec4> whiskerColors;
  std::vector<glm::vec4> outlineColors;

  int pointsPerBox;
  double boxWidth;

 public:
  BoxChartWidget(Canvas* c, double x, double y, double w, double h)
      : GraphWidget(c, x, y, w, h, DISCRETE_PERMITTED, FUNCTIONS_PERMITTED),
        whiskerStyle(nullptr),
        boxStyle(nullptr),
        data(std::vector<double>()),
        names(std::vector<std::string>()),
        boxColors({grail::blue}),
        whiskerColors({grail::black}),
        outlineColors({grail::black}),
        boxWidth(20),
        pointsPerBox(3) {}

	void setStyle(const GraphStyle* s);
  void setWhiskerStyle(const Style* s);
  void setBoxStyle(const Style* s);
  void setBoxWidth(double width);
  void setBoxColors(std::vector<glm::vec4>& colors);
  void setWhiskerColors(std::vector<glm::vec4>& colors);
  void setOutlineColors(std::vector<glm::vec4>& colors);
  void setPointsPerBox(int n);
  void setData(const std::vector<double>& data);
  void setNames(const std::vector<std::string>& names);
  void init() override;
};

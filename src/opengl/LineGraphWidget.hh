#pragma once

#include "opengl/GraphWidget.hh"

class LineGraphWidget : public GraphWidget {
 private:
  const Style *dataStyle;
  std::vector<double> xPoints;
  std::vector<double> yPoints;
  glm::vec4 &pointColor;
  double pointSize;
  // floats used here because its what StyledMultiShape wants
  void (StyledMultiShape2D::*markerFunction)(float, float, float, glm::vec4 &);
  std::unordered_map<char, void (StyledMultiShape2D::*)(float, float, float,
                                                        glm::vec4 &)>
      map;

 public:
  LineGraphWidget(Canvas *c, double x, double y, double w, double h)
      : GraphWidget(c, x, y, w, h),
        dataStyle(new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0, 1)),
        xPoints(std::vector<double>()),
        yPoints(std::vector<double>()),
        pointColor(grail::blue),
        pointSize(4),
        markerFunction(&StyledMultiShape2D::drawCircleMarker) {
    map['o'] = &StyledMultiShape2D::drawCircleMarker;
    map['s'] = &StyledMultiShape2D::drawSquareMarker;
    map['h'] = &StyledMultiShape2D::drawHexagonMarker;
    map['t'] = &StyledMultiShape2D::drawTriangleMarker;
    map['p'] = &StyledMultiShape2D::drawPentagonMarker;
    map['c'] = &StyledMultiShape2D::drawCrossMarker;
  }

  void setDataStyle(const Style *s);
  void setPointFormat(char pt, double size, glm::vec4 &color);
  void setXPoints(const std::vector<double> &xPoints);
  void setYPoints(const std::vector<double> &yPoints);
  void createXAxis(AxisType a) override;
  void createYAxis(AxisType a) override;
  void init() override;
};

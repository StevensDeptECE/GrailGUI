#pragma once

#include "opengl/GraphWidget.hh"

class LineGraphWidget : public GraphWidget {
 private:
  const Style* dataStyle;
  std::vector<double> xPoints;
  std::vector<double> yPoints;
  glm::vec4& pointColor;
  double pointSize;
  // floats used here because its what StyledMultiShape wants
  void (StyledMultiShape2D::*markerFunction)(float, float, float, glm::vec4&);
  std::unordered_map<char, void (StyledMultiShape2D::*)(float, float, float,
                                                        glm::vec4&)>
      marker_pointer_table;

 public:
  LineGraphWidget(Canvas* c, double x, double y, double w, double h)
      : GraphWidget(c, x, y, w, h, FUNCTIONS_PERMITTED, FUNCTIONS_PERMITTED),
        dataStyle(nullptr),
        xPoints(),
        yPoints(),
        pointColor(grail::blue),
        pointSize(4),
        markerFunction(&StyledMultiShape2D::drawCircleMarker) {
    marker_pointer_table['o'] = &StyledMultiShape2D::drawCircleMarker;
    marker_pointer_table['s'] = &StyledMultiShape2D::drawSquareMarker;
    marker_pointer_table['h'] = &StyledMultiShape2D::drawHexagonMarker;
    marker_pointer_table['t'] = &StyledMultiShape2D::drawTriangleMarker;
    marker_pointer_table['p'] = &StyledMultiShape2D::drawPentagonMarker;
    marker_pointer_table['c'] = &StyledMultiShape2D::drawCrossMarker;
  }

  void setDataStyle(const Style* s);
  void setPointFormat(char pt, double size, glm::vec4& color);
  void setXPoints(const std::vector<double>& xPoints);
  void setYPoints(const std::vector<double>& yPoints);
  void init() override;
};

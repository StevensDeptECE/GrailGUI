#pragma once

#include "opengl/GraphWidget.hh"

class LineGraphWidget : public GraphWidget {
 private:
  std::vector<double> xPoints;
  std::vector<double> yPoints;
  double pointSize;
  // floats used here because its what StyledMultiShape wants
	typedef void (StyledMultiShape2D::*MarkerFunction)(float, float, float, const glm::vec4&);
  MarkerFunction                         markerFunction;
  std::unordered_map<char, MarkerFunction> marker_pointer_table;

 public:
  LineGraphWidget(Canvas* c, double x, double y, double w, double h, AxisType xAxisType, AxisType yAxisType, const GraphStyle* s)
      : GraphWidget(c, x, y, w, h, FUNCTIONS_PERMITTED, FUNCTIONS_PERMITTED, xAxisType, yAxisType, s),
        xPoints(),
        yPoints(),
        markerFunction(&StyledMultiShape2D::drawCircleMarker) {
    marker_pointer_table['o'] = &StyledMultiShape2D::drawCircleMarker;
    marker_pointer_table['s'] = &StyledMultiShape2D::drawSquareMarker;
    marker_pointer_table['h'] = &StyledMultiShape2D::drawHexagonMarker;
    marker_pointer_table['t'] = &StyledMultiShape2D::drawTriangleMarker;
    marker_pointer_table['p'] = &StyledMultiShape2D::drawPentagonMarker;
    marker_pointer_table['c'] = &StyledMultiShape2D::drawCrossMarker;
  }

  void setPointFormat(char pt, double size, glm::vec4& color);
  void setXPoints(const std::vector<double>& xPoints);
  void setYPoints(const std::vector<double>& yPoints);
  void init() override;
};

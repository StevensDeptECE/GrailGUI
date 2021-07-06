#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "opengl/AxisWidget.hh"
#include "opengl/Errcode.hh"
#include "opengl/Scale.hh"
#include "opengl/SuperWidget2D.hh"
#include "util/Ex.hh"

class Style;

enum AxisType {
  linear,
  logar,
};

class LineGraphWidget : public SuperWidget2D {
 private:
  std::vector<double> xPoints;
  std::vector<double> yPoints;
  std::string graphTitle;
  glm::vec4 &pointColor;
  double pointSize;
  void (StyledMultiShape2D::*markerFunction)(float, float, float, glm::vec4 &);
  const Style *xAxisTextStyle;
  const Style *yAxisTextStyle;
  AxisType xAxisType;
  AxisType yAxisType;

  std::unordered_map<char, void (StyledMultiShape2D::*)(float, float, float,
                                                        glm::vec4 &)>
      map;

 public:
  AxisWidget *xAxis;  // can be either linear or logarithmic
  AxisWidget *yAxis;

  LineGraphWidget(Canvas *c, StyledMultiShape2D *m, MultiText *t, float x,
                  float y, float w, float h)
      : SuperWidget2D(c, m, t, x, y, w, h),
        xPoints(std::vector<double>()),
        yPoints(std::vector<double>()),
        xAxisTextStyle(new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0)),
        yAxisTextStyle(new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0)),
        pointColor(grail::blue),
        pointSize(4),
        markerFunction(&StyledMultiShape2D::drawCircleMarker),
        xAxis(nullptr),
        yAxis(nullptr) {
    map['o'] = &StyledMultiShape2D::drawCircleMarker;
    map['s'] = &StyledMultiShape2D::drawSquareMarker;
    map['h'] = &StyledMultiShape2D::drawHexagonMarker;
    map['t'] = &StyledMultiShape2D::drawTriangleMarker;
    map['p'] = &StyledMultiShape2D::drawPentagonMarker;
    map['c'] = &StyledMultiShape2D::drawCrossMarker;
  }
  void setPointFormat(char pt, double size, glm::vec4 &color);
  void setXPoints(const std::vector<double> &xPoints);
  void setYPoints(const std::vector<double> &yPoints);
  void createXAxis(AxisType a);
  void createYAxis(AxisType a);
  void setGraphTitle(std::string text);
  void setXAxisTextStyle(const Style *xAxisTextStyle);
  void setYAxisTextStyle(const Style *yAxisTextStyle);
#if 0
    //I've left this here from whoever was working with line graph previously,
    //it needs to be ported to the new format/api

    std::string text;
  const Style *titleStyle;
  const Style *barStyle;
  std::unique_ptr<Scale> xAxisScale;
  std::unique_ptr<Scale> yAxisScale;
  std::vector<glm::vec4> colors;
  std::vector<std::string> names;
  int startPoint, endPoint;
  Scale *yAxis;
  Scale *xAxis;

 public:
  LineGraphWidget(StyledMultiShape2D *m, MultiText *t, float x, float y,
                  float w, float h)
      : Widget2D(m, t, x, y, w, h), text(), titleStyle(nullptr) {}
  void setTitleStyle(const Style *s) { titleStyle = s; }
  void chart(float xMax, float yMax, float xInterval, float yInterval,
             Scale *xAxis, Scale *yAxis, int start, int end);
  // void chartLog(const float b[], int size, float relativeSpace,
  //             const std::string barNames[], int logBase);
  void add(const std::vector<float> &xPoints, const std::vector<float> &yPoints,
           const glm::vec4 &rgb, std::string name);
  void title(const std::string &s);
  void axes(char xLabel[], int xLen, char yLabel[], int yLen, float xMax,
            float yMax, float xMin, float yMin, float xInterval,
            float yInterval);
  void axes(char xLabel[], int xLen, char yLabel[], int yLen, float xMax,
            float yMax, float xMin, float yMin, float xInterval,
            float yInterval, int start, int end);
  void legend(float x, float y);
#endif
  void init() override;
};

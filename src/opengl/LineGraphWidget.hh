#pragma once

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
  const Style *xAxisTextStyle;
  const Style *yAxisTextStyle;
  AxisType xAxisType;
  AxisType yAxisType;

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
        xAxis(nullptr),
        yAxis(nullptr) {}
  void setXPoints(const std::vector<double> &xPoints);
  void setYPoints(const std::vector<double> &yPoints);
  void createXAxis(AxisType a);
  void createYAxis(AxisType a);
  void setGraphTitle(std::string text);
  void setXAxisTextStyle(const Style *xAxisTextStyle);
  void setYAxisTextStyle(const Style *yAxisTextStyle);
  void init() override;
};

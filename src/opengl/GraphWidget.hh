#pragma once

#include <string>
#include <vector>

#include "opengl/AxisWidget.hh"
#include "opengl/SuperWidget2D.hh"

class Style;

class GraphWidget : public SuperWidget2D {
 public:
  enum AxisType {
    LINEAR,
    LOGARITHMIC,
    TEXT,
  };

 protected:
  std::string graphTitle;
  const Style *xAxisTextStyle;
  const Style *yAxisTextStyle;
  AxisType xAxisType;
  AxisType yAxisType;

 public:
  AxisWidget *xAxis;
  AxisWidget *yAxis;

  GraphWidget(Canvas *c, StyledMultiShape2D *m, MultiText *t, float x, float y,
              float w, float h)
      : SuperWidget2D(c, m, t, x, y, w, h),
        xAxisTextStyle(new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0)),
        yAxisTextStyle(new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0)),
        xAxis(nullptr),
        yAxis(nullptr) {}
  virtual void createXAxis(AxisType a) = 0;
  virtual void createYAxis(AxisType a) = 0;
  void setGraphTitle(std::string text);
  void setXAxisTextStyle(const Style *xAxisTextStyle);
  void setYAxisTextStyle(const Style *yAxisTextStyle);
};

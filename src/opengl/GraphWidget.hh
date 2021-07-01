#pragma once

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
  const Style *baseStyle;
  const Style *xAxisStyle;
  const Style *xAxisTextStyle;
  const Style *yAxisStyle;
  const Style *yAxisTextStyle;
  AxisType xAxisType;
  AxisType yAxisType;

  void commonRender();

 public:
  AxisWidget *xAxis;
  AxisWidget *yAxis;

  GraphWidget(Canvas *c, double x, double y, double w, double h,
              Style *baseStyle = nullptr, Style *xAxisStyle = nullptr,
              Style *xAxisTextStyle = nullptr, Style *yAxisStyle = nullptr,
              Style *yAxisTextStyle = nullptr);
  ~GraphWidget();

  virtual void createXAxis(AxisType a) = 0;
  virtual void createYAxis(AxisType a) = 0;
  void setBaseStyle(const Style *s);
  void setGraphTitle(std::string text);
  void setXAxisTextStyle(const Style *xAxisTextStyle);
  void setYAxisTextStyle(const Style *yAxisTextStyle);
  void setXAxisStyle(const Style *xAxisStyle);
  void setYAxisStyle(const Style *yAxisStyle);
  virtual void init() = 0;
};

#pragma once

#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/Widget2D.hh"
// potential includes for error handling in the future
// #include "util/Ex.hh"
// #include "opengl/Errcode.hh"

class AxisWidget : public Widget2D {
 protected:
  double minBound;
  double maxBound;
  double tickInterval;
  std::string axisTitle;
  const Style *axisTickStyle;
  const Style *axisTitleStyle;

 public:
  // TODO: initialize title and other style bits to some kind of sane default
  AxisWidget(StyledMultiShape2D *m, MultiText *t, float x, float y, float w,
             float h)
      : Widget2D(m, t, x, y, w, h),
        minBound(0),
        maxBound(0),
        tickInterval(1),
        axisTitle(nullptr),
        axisTickStyle(nullptr),
        axisTitleStyle(nullptr) {}
  void setBounds(double minBound, double maxBound);
  void setTickInterval(double TickInterval);
  void setTitle(std::string text);
  void setTickStyle(const Style *style);
  void setTitleStyle(const Style *style);
};

class LinearAxisWidget : public AxisWidget {
 public:
  LinearAxisWidget(StyledMultiShape2D *m, MultiText *t, float x, float y,
                   float w, float h)
      : AxisWidget(m, t, x, y, w, h){};
  void init() override;
};
class LogAxisWidget : public AxisWidget {};
class TextAxisWidget : public AxisWidget {};

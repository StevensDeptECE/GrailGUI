#pragma once

#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/Widget2D.hh"
// potential includes for error handling in the future
// #include "util/Ex.hh"
// #include "opengl/Errcode.hh"

class AxisWidget : public Widget2D {
 protected:
  struct Format {
    int width;
    int precision;
  };

  double minBound;                  // minimum axis value
  double maxBound;                  // maximum axis value
  double tickInterval;              // interval between ticks
  double tickDrawSize;              // vertical height of tick marks
  Format tickFormat;                // how to format float values on screen
  bool showTicks;                   // whether or not to show the ticks
  std::string axisTitle;            // axis title
  glm::vec4 axisColor;              // color to draw the axis line
  glm::vec4 tickColor;              // color to draw the ticks
  const Style *axisTickLabelStyle;  // style of the tick labels
  const Style *axisTitleStyle;      // style of the title

  double bottomOffset;
  void addAxisTitle();
  virtual void setBounds(double minBound, double maxBound) = 0;
  virtual void setTickInterval(double tickInterval) = 0;

 public:
  // TODO: initialize title and other style bits to some kind of sane default
  AxisWidget(StyledMultiShape2D *m, MultiText *t, float x, float y, float w,
             float h, double minBound = 0, double maxBound = 0,
             double tickInterval = 1, double tickDrawSize = 5,
             bool showTicks = true, std::string axisTitle = "",
             const glm::vec4 &axisColor = grail::black,
             const glm::vec4 &tickColor = grail::black,
             const Style *axisTitleStyle = nullptr,
             const Style *axisTickStyle = nullptr, int tickFormatWidth = 2,
             int tickFormatPrecision = 2);

  void setTickDrawSize(double i);
  void setShowTicks(bool b);
  void setTitle(std::string text);
  void setAxisColor(const glm::vec4 &color);
  void setTickColor(const glm::vec4 &color);
  void setTitleStyle(const Style *style);
  void setTickLabelStyle(const Style *style);
  void setTickFormat(int width, int precision);
};

class LinearAxisWidget : public AxisWidget {
 public:
  LinearAxisWidget(StyledMultiShape2D *m, MultiText *t, float x, float y,
                   float w, float h);
  void setBounds(double minBound, double maxBound) override;
  void setTickInterval(double tickInterval) override;
  void init() override;
};

class LogAxisWidget : public AxisWidget {
 private:
  int base, power;

 public:
  LogAxisWidget(StyledMultiShape2D *m, MultiText *t, float x, float y, float w,
                float h);
  void setBounds(double minBound, double maxBound) override;
  void setTickInterval(double tickInterval) override;
  void init() override;
};

class TextAxisWidget : public AxisWidget {
 private:
  using AxisWidget::setBounds;
  using AxisWidget::setTickInterval;
  void setBounds(double minBound, double maxBound) override{};
  void setTickInterval(double tickInterval) override{};
  std::vector<std::string> tickLabels;

 public:
  TextAxisWidget(StyledMultiShape2D *m, MultiText *t, float x, float y, float w,
                 float h);
  void setTickLabels(std::vector<std::string> tickLabels);
  void init() override;
};

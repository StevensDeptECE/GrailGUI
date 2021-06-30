#pragma once

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

  double minBound;        // minimum axis value
  double maxBound;        // maximum axis value
  double tickInterval;    // interval between ticks
  double tickDrawSize;    // vertical height of tick marks
  Format tickFormat;      // how to format float values on screen
  bool showTicks;         // whether or not to show the ticks
  bool isVert;            // whether the axis will be drawn vertically
  std::string axisTitle;  // axis title
  glm::vec4 axisColor;    // color to draw the axis line
  glm::vec4 tickColor;    // color to draw the ticks

  double bottomOffset;
  void addAxisTitle();

 public:
  // TODO: initialize title and other style bits to some kind of sane default
  AxisWidget(StyledMultiShape2D *m, MultiText *t, double x, double y, double w,
             double h, double minBound = 0, double maxBound = 0,
             double tickInterval = 1, double tickDrawSize = 5,
             bool showTicks = true, bool isVert = false,
             std::string axisTitle = "",
             const glm::vec4 &axisColor = grail::black,
             const glm::vec4 &tickColor = grail::black, int tickFormatWidth = 2,
             int tickFormatPrecision = 2, double bottomOffset = 0);

  void setTickDrawSize(double i);
  void setShowTicks(bool b);
  void setIsVert(bool b);
  void setTitle(std::string text);
  void setAxisColor(const glm::vec4 &color);
  void setTickColor(const glm::vec4 &color);
  void setTickFormat(int width, int precision);
  double getTickInterval();
  double getMinBound();
  double getMaxBound();
  virtual void setBounds(double minBound, double maxBound) = 0;
  virtual void setTickInterval(double tickInterval) = 0;
};

class LinearAxisWidget : public AxisWidget {
 public:
  LinearAxisWidget(StyledMultiShape2D *m, MultiText *t, double x, double y,
                   double w, double h);
  void setBounds(double minBound, double maxBound) override;
  void setTickInterval(double tickInterval) override;
  void init() override;
};

class LogAxisWidget : public AxisWidget {
 private:
  int base, power;

 public:
  LogAxisWidget(StyledMultiShape2D *m, MultiText *t, double x, double y, double w,
                double h);
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
  TextAxisWidget(StyledMultiShape2D *m, MultiText *t, double x, double y, double w,
                 double h);
  void setTickLabels(std::vector<std::string> tickLabels);
  void init() override;
};

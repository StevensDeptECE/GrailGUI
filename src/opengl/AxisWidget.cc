#include "opengl/AxisWidget.hh"

#include <cmath>

using namespace std;

AxisWidget::AxisWidget(StyledMultiShape2D *m, MultiText *t, float x, float y,
                       float w, float h, double minBound, double maxBound,
                       double tickInterval, double tickDrawSize, bool showTicks,
                       std::string axisTitle, const glm::vec4 &axisColor,
                       const glm::vec4 &tickColor, const Style *axisTitleStyle,
                       const Style *axisTickStyle, int tickFormatWidth,
                       int tickFormatPrecision)
    : Widget2D(m, t, x, y, w, h),
      minBound(minBound),
      maxBound(maxBound),
      tickInterval(tickInterval),
      tickDrawSize(tickDrawSize),
      showTicks(showTicks),
      axisTitle(axisTitle),
      axisColor(axisColor),
      tickColor(tickColor),
      axisTitleStyle(axisTitleStyle),
      axisTickLabelStyle(axisTickStyle),
      tickFormat({tickFormatWidth, tickFormatPrecision}) {}

void AxisWidget::setTickDrawSize(double i) { tickDrawSize = i; }

void AxisWidget::setShowTicks(bool b) { showTicks = b; }

void AxisWidget::setTitle(std::string text) { axisTitle = text; }

void AxisWidget::setAxisColor(const glm::vec4 &color) { axisColor = color; }

void AxisWidget::setTickColor(const glm::vec4 &color) { tickColor = color; }

void AxisWidget::setTickLabelStyle(const Style *style) {
  axisTickLabelStyle = style;
}

void AxisWidget::setTitleStyle(const Style *style) { axisTitleStyle = style; }

void AxisWidget::addAxisTitle() {
  if (axisTitle.size()) {
    t->addCentered(x, y + h + (bottomOffset += 10), w,
                   axisTitleStyle->f->getHeight() + 10, axisTitleStyle->f,
                   axisTitle.c_str(), axisTitle.size());
  }
}

LinearAxisWidget::LinearAxisWidget(StyledMultiShape2D *m, MultiText *t, float x,
                                   float y, float w, float h)
    : AxisWidget(m, t, x, y, w, h) {}

void LinearAxisWidget::setTickFormat(int width, int precision) {
  tickFormat = {width, precision};
}

void LinearAxisWidget::setBounds(double minBound, double maxBound) {
  this->minBound = minBound;
  this->maxBound = maxBound;
}

void LinearAxisWidget::setTickInterval(double tickInterval) {
  this->tickInterval = tickInterval;
}

void LinearAxisWidget::init() {
  float scale = w / abs(maxBound - minBound);
  bottomOffset = tickDrawSize + axisTickLabelStyle->f->getHeight();
  m->drawLine(x, y + h, x + scale * maxBound, y + h, axisColor);

  for (float tick = minBound; tick <= maxBound; tick += tickInterval) {
    float draw = x + scale * tick;

    if (showTicks)
      m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                  tickColor);

    t->add(draw, y + h + bottomOffset, axisTickLabelStyle->f, tick,
           tickFormat.width, tickFormat.precision);
  }

  addAxisTitle();
}

LogAxisWidget::LogAxisWidget(StyledMultiShape2D *m, MultiText *t, float x,
                             float y, float w, float h)
    : AxisWidget(m, t, x, y, w, h), base(10), power(0) {}

void LogAxisWidget::setTickFormat(int width, int precision) {
  tickFormat = {width, precision};
}

void LogAxisWidget::setScale(int base, int power) {
  this->base = base;
  this->power = power;
}

void LogAxisWidget::setNumTicks(int num) {
  minBound = 0;
  maxBound = 10;
  tickInterval = maxBound / num;
}

void LogAxisWidget::init() {
  float scale = w / abs(maxBound - minBound);
  bottomOffset = tickDrawSize + axisTickLabelStyle->f->getHeight();
  m->drawLine(x, y + h, x + scale * maxBound, y + h, axisColor);

  for (float tick = minBound; tick <= maxBound; tick += tickInterval) {
    float draw = x + scale * tick;

    if (showTicks)
      m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                  tickColor);

    t->add(draw, y + h + bottomOffset, axisTickLabelStyle->f, pow(base, power),
           tickFormat.width, tickFormat.precision);
    power++;
  }

  addAxisTitle();
}

TextAxisWidget::TextAxisWidget(StyledMultiShape2D *m, MultiText *t, float x,
                               float y, float w, float h)
    : AxisWidget(m, t, x, y, w, h), tickLabels(vector<string>()) {}

void TextAxisWidget::setTickLabels(vector<string> tickLabels) {
  this->tickLabels = tickLabels;
}

void TextAxisWidget::init() {
  minBound = 0;
  maxBound = tickLabels.size() * (tickLabels.size() + 1);
  tickInterval = tickLabels.size();
  bottomOffset = tickDrawSize + axisTickLabelStyle->f->getHeight();
  float scale = w / abs(maxBound - minBound);
  int index = 0;

  m->drawLine(x, y + h, x + scale * maxBound, y + h, axisColor);

  for (float tick = minBound + tickInterval; tick < maxBound;
       tick += tickInterval) {
    float draw = x + scale * tick;

    if (showTicks)
      m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                  tickColor);

    string current = tickLabels[index];
    t->add(draw, y + h + bottomOffset, axisTickLabelStyle->f, current.c_str(),
           strlen(current.c_str()));

    index++;
  }

  addAxisTitle();
}

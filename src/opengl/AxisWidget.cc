#include "opengl/AxisWidget.hh"

#include <cmath>

using namespace std;

AxisWidget::AxisWidget(StyledMultiShape2D *m, MultiText *t, float x, float y,
                       float w, float h, double minBound, double maxBound,
                       double tickInterval, double tickDrawSize, bool showTicks,
                       std::string axisTitle, const Style *axisTitleStyle,
                       const Style *axisTickStyle, int tickFormatWidth,
                       int tickFormatPrecision)
    : Widget2D(m, t, x, y, w, h),
      minBound(minBound),
      maxBound(maxBound),
      tickInterval(tickInterval),
      tickDrawSize(tickDrawSize),
      showTicks(showTicks),
      axisTitle(axisTitle),
      axisTitleStyle(axisTitleStyle),
      axisTickStyle(axisTickStyle),
      tickFormat({tickFormatWidth, tickFormatPrecision}) {}

void AxisWidget::setTickDrawSize(double i) { this->tickDrawSize = i; }


void AxisWidget::setShowTicks(bool b) { this->showTicks = b; }

void AxisWidget::setTitle(std::string text) { this->axisTitle = text; }

void AxisWidget::setTickStyle(const Style *style) {
  this->axisTickStyle = style;
}

void AxisWidget::setTitleStyle(const Style *style) {
  this->axisTitleStyle = style;
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
  m->drawLine(x, y + h, x + scale * maxBound, y + h, grail::black);

  for (float tick = minBound; tick <= maxBound; tick += tickInterval) {
    float draw = x + scale * tick;
    if (showTicks)
      m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                  grail::black);
    t->add(draw, y + h + tickDrawSize * 4, axisTickStyle->f, tick,
           tickFormat.width, tickFormat.precision);
  }

  if (axisTitle.size()) {
    t->addCentered(w / 2, y + h + tickDrawSize * 8, 200, 0, axisTitleStyle->f,
                   axisTitle.c_str(), axisTitle.size());
  }
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
  m->drawLine(x, y + h, x + scale * maxBound, y + h, grail::black);

  for (float tick = minBound; tick <= maxBound; tick += tickInterval) {
    float draw = x + scale * tick;
    if (showTicks)
      m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                  grail::black);

    t->add(draw, y + h + tickDrawSize * 4, axisTickStyle->f, pow(base, power),
           tickFormat.width, tickFormat.precision);
    power++;
  }

  // if (axisTitle.size()) {
  //   t->add(w / 2, y + h + tickDrawSize * 4, axisTitleStyle->f,
  //   axisTitle.c_str(), axisTitle.size());
  // }
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
  float scale = w / abs(maxBound - minBound);
  int index = 0;

  m->drawLine(x, y + h, x + scale * maxBound, y + h, grail::black);

  for (float tick = minBound + tickInterval; tick < maxBound;
       tick += tickInterval) {
    float draw = x + scale * tick;

    if (showTicks)
      m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                  grail::black);

    string current = tickLabels[index];
    t->add(draw, y + h + tickDrawSize * 4, axisTickStyle->f, current.c_str(),
           strlen(current.c_str()));

    index++;
  }
}

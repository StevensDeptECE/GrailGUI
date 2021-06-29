#include "opengl/AxisWidget.hh"

using namespace std;

AxisWidget::AxisWidget(StyledMultiShape2D *m, MultiText *t, double x, double y,
                       double w, double h, double minBound, double maxBound,
                       double tickInterval, double tickDrawSize, bool showTicks,
                       bool isVert, std::string axisTitle,
                       const glm::vec4 &axisColor, const glm::vec4 &tickColor,
                       int tickFormatWidth, int tickFormatPrecision)
    : Widget2D(m, t, x, y, w, h),
      minBound(minBound),
      maxBound(maxBound),
      tickInterval(tickInterval),
      tickDrawSize(tickDrawSize),
      showTicks(showTicks),
      isVert(isVert),
      axisTitle(axisTitle),
      axisColor(axisColor),
      tickColor(tickColor),
      tickFormat({tickFormatWidth, tickFormatPrecision}) {}

void AxisWidget::setTickDrawSize(double i) { tickDrawSize = i; }

void AxisWidget::setShowTicks(bool b) { showTicks = b; }

void AxisWidget::setIsVert(bool b) { isVert = b; }

void AxisWidget::setTitle(std::string text) { axisTitle = text; }

void AxisWidget::setAxisColor(const glm::vec4 &color) { axisColor = color; }

void AxisWidget::setTickColor(const glm::vec4 &color) { tickColor = color; }

void AxisWidget::setTickFormat(int width, int precision) {
  tickFormat = {width, precision};
}

double AxisWidget::getTickInterval() { return tickInterval; }

double AxisWidget::getMinBound() { return minBound; }

double AxisWidget::getMaxBound() { return maxBound; }

void AxisWidget::addAxisTitle() {
  if (axisTitle.size()) {
    t->addCentered(
        x,
        y + h +
            ((isVert) ? (bottomOffset -= 20 + m->getStyle()->f->getHeight())
                      : (bottomOffset += 10)),
        w, m->getStyle()->f->getHeight() + 10, m->getStyle()->f,
        axisTitle.c_str(), axisTitle.size());
  }
}

LinearAxisWidget::LinearAxisWidget(StyledMultiShape2D *m, MultiText *t,
                                   double x, double y, double w, double h)
    : AxisWidget(m, t, x, y, w, h) {}

void LinearAxisWidget::setBounds(double minBound, double maxBound) {
  this->minBound = minBound;
  this->maxBound = maxBound;
}

void LinearAxisWidget::setTickInterval(double tickInterval) {
  this->tickInterval = tickInterval;
}

void LinearAxisWidget::init() {
  double scale = w / abs(maxBound - minBound);
  bottomOffset = tickDrawSize + m->getStyle()->f->getHeight();
  if (isVert) bottomOffset = -bottomOffset;
  m->drawLine(x, y + h, x + w, y + h, axisColor);

  double counter = 0;
  for (double tick = minBound; tick <= maxBound;
       tick += tickInterval, counter += tickInterval) {
    double draw = x + scale * counter;

    if (showTicks)
      m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                  tickColor);

    t->add(draw, y + h + bottomOffset, m->getStyle()->f, tick, tickFormat.width,
           tickFormat.precision);
  }

  addAxisTitle();
}

LogAxisWidget::LogAxisWidget(StyledMultiShape2D *m, MultiText *t, double x,
                             double y, double w, double h)
    : AxisWidget(m, t, x, y, w, h), base(10), power(0) {}

void LogAxisWidget::setBounds(double minBound, double maxBound) {
  this->minBound = minBound;
  this->maxBound = maxBound;
}

void LogAxisWidget::setTickInterval(double tickInterval) {
  this->tickInterval = tickInterval;
}

void LogAxisWidget::init() {
  double base = 1 / log(tickInterval);
  double scale = w / abs(log(maxBound) * base - log(minBound) * base);
  bottomOffset = tickDrawSize + m->getStyle()->f->getHeight();
  if (isVert) bottomOffset = -bottomOffset;
  m->drawLine(x, y + h, x + w, y + h, axisColor);

  double counter = 1;
  for (double tick = minBound; tick <= maxBound;
       tick *= tickInterval, counter *= tickInterval) {
    double draw = x + scale * log(counter) * base;

    if (showTicks)
      m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                  tickColor);

    t->add(draw, y + h + bottomOffset, m->getStyle()->f, tick, tickFormat.width,
           tickFormat.precision);
  }

  addAxisTitle();
}

TextAxisWidget::TextAxisWidget(StyledMultiShape2D *m, MultiText *t, double x,
                               double y, double w, double h)
    : AxisWidget(m, t, x, y, w, h), tickLabels(vector<string>()) {}

void TextAxisWidget::setTickLabels(vector<string> tickLabels) {
  this->tickLabels = tickLabels;
}

void TextAxisWidget::init() {
  minBound = 0;
  maxBound = tickLabels.size() * (tickLabels.size() + 1);
  tickInterval = tickLabels.size();
  bottomOffset = tickDrawSize + m->getStyle()->f->getHeight();
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
    t->add(draw, y + h + bottomOffset, m->getStyle()->f, current.c_str(),
           strlen(current.c_str()));

    index++;
  }

  addAxisTitle();
}

#include "opengl/AxisWidget.hh"

using namespace std;

AxisWidget::AxisWidget(StyledMultiShape2D* m, MultiText* t, double x, double y,
                       double w, double h, double minBound, double maxBound,
                       double tickInterval, double tickDrawSize, bool showTicks,
                       bool isVert, const std::string& axisTitle,
                       int tickFormatWidth, int tickFormatPrecision,
                       double bottomOffset)
    : Widget2D(m, t, x, y, w, h),
      minBound(minBound),
      maxBound(maxBound),
      tickInterval(tickInterval),
      tickDrawSize(tickDrawSize),
      showTicks(showTicks),
      isVert(isVert),
      axisTitle(axisTitle),
			tickFormat({.width = tickFormatWidth, .precision = tickFormatPrecision}),
      bottomOffset(bottomOffset) {}

AxisWidget::~AxisWidget() {}

void AxisWidget::setTickDrawSize(double i) { tickDrawSize = i; }

void AxisWidget::setShowTicks(bool b) { showTicks = b; }

void AxisWidget::setIsVert(bool b) { isVert = b; }

void AxisWidget::setTitle(std::string text) { axisTitle = text; }

void AxisWidget::setAxisColor(const glm::vec4& color) { axisColor = color; }

void AxisWidget::setTickColor(const glm::vec4& color) { tickColor = color; }

void AxisWidget::setTickFormat(int width, int precision) {
  tickFormat = {width, precision};
}

double AxisWidget::getTickInterval() { return tickInterval; }

double AxisWidget::getMinBound() { return minBound; }

double AxisWidget::getMaxBound() { return maxBound; }

void AxisWidget::addAxisTitle() {
  if (axisTitle.size()) {
    if (isVert) {
      t->addCentered(x - 1.5 * m->getStyle()->f->getWidth(axisTitle.c_str(),
                                                          axisTitle.size()),
                     y + w / 2, m->getStyle()->f, axisTitle.c_str(),
                     axisTitle.size());
    } else {
      t->addCentered(
          x + w / 2,
          y + h + (bottomOffset += 10 + m->getStyle()->f->getHeight()),
          m->getStyle()->f, axisTitle.c_str(), axisTitle.size());
    }
  }
}

LinearAxisWidget::LinearAxisWidget(StyledMultiShape2D* m, MultiText* t,
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
  bottomOffset = tickDrawSize + m->getStyle()->f->getHeight() * 1.25;
  m->drawLine(x, y + h, x + w, y + h, axisColor);

  if (isVert) {
    double counter = 0;
    for (double tick = maxBound; tick >= minBound;
         tick -= tickInterval, counter += tickInterval) {
      double draw = y + scale * counter;

      if (showTicks)
        m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                    tickColor);

      char fmt[50];
      sprintf(fmt, "%%%d.%dlf", tickFormat.width, tickFormat.precision);

      char thing[50];
      sprintf(thing, fmt, tick);

      t->add(x - 20 - m->getStyle()->f->getWidth(thing, strlen(thing)),
             draw + m->getStyle()->f->getHeight() / 2, m->getStyle()->f, tick,
             tickFormat.width, tickFormat.precision);
      // t->addCentered(x, draw + m->getStyle()->f->getHeight() / 2,
      //                m->getStyle()->f, tick, tickFormat.width,
      //                tickFormat.precision);
    }
  } else {
    double counter = 0;
    for (double tick = minBound; tick <= maxBound;
         tick += tickInterval, counter += tickInterval) {
      double draw = x + scale * counter;

      if (showTicks)
        m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                    tickColor);

      t->addCentered(draw, y + h + bottomOffset, m->getStyle()->f, tick,
                     tickFormat.width, tickFormat.precision);
    }
  }

  addAxisTitle();
}

LogAxisWidget::LogAxisWidget(StyledMultiShape2D* m, MultiText* t, double x,
                             double y, double w, double h)
    : AxisWidget(m, t, x, y, w, h) {}

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
  bottomOffset = tickDrawSize + m->getStyle()->f->getHeight() * 1.25;
  m->drawLine(x, y + h, x + w, y + h, axisColor);

  if (isVert) {
    double counter = 1;
    for (double tick = maxBound; tick >= minBound;
         tick /= tickInterval, counter *= tickInterval) {
      double draw = y + scale * log(counter) * base;

      if (showTicks)
        m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                    tickColor);

      char fmt[50];
      sprintf(fmt, "%%%d.%dlf", tickFormat.width, tickFormat.precision);

      char thing[50];
      sprintf(thing, fmt, tick);

      t->add(x - 20 - m->getStyle()->f->getWidth(thing, strlen(thing)),
             draw + m->getStyle()->f->getHeight() / 2, m->getStyle()->f, tick,
             tickFormat.width, tickFormat.precision);
    }
  } else {
    double counter = 1;
    for (double tick = minBound; tick <= maxBound;
         tick *= tickInterval, counter *= tickInterval) {
      double draw = x + scale * log(counter) * base;

      if (showTicks)
        m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                    tickColor);

      t->addCentered(draw, y + h + bottomOffset, m->getStyle()->f, tick,
                     tickFormat.width, tickFormat.precision);
    }
  }

  addAxisTitle();
}

TextAxisWidget::TextAxisWidget(StyledMultiShape2D* m, MultiText* t, double x,
                               double y, double w, double h)
    : AxisWidget(m, t, x, y, w, h), tickLabels(vector<string>()) {}

void TextAxisWidget::setTickLabels(vector<string> tickLabels) {
  this->tickLabels = tickLabels;
}

void TextAxisWidget::init() {
  minBound = 0;
  maxBound = tickLabels.size() * (tickLabels.size() + 1);
  tickInterval = tickLabels.size();
  bottomOffset = tickDrawSize + m->getStyle()->f->getHeight() * 1.25;
  float scale = w / abs(maxBound - minBound);
  int index = 0;

  m->drawLine(x, y + h, x + scale * maxBound, y + h, axisColor);

  if (isVert) {
    for (float tick = maxBound - tickInterval; tick > minBound;
         tick -= tickInterval) {
      float draw = y + scale * tick;

      if (showTicks)
        m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                    tickColor);

      string current = tickLabels[index];
      t->add(x, draw, m->getStyle()->f, current.c_str(),
             strlen(current.c_str()));

      index++;
    }
  } else {
    for (float tick = minBound + tickInterval; tick < maxBound;
         tick += tickInterval) {
      float draw = x + scale * tick;

      if (showTicks)
        m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                    tickColor);

      string current = tickLabels[index];
      t->addCentered(draw, y + h + bottomOffset, m->getStyle()->f,
                     current.c_str(), strlen(current.c_str()));

      index++;
    }
  }

  addAxisTitle();
}

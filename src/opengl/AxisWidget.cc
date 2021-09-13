#include "opengl/AxisWidget.hh"
#include <cmath>

using namespace std;

AxisWidget::AxisWidget(StyledMultiShape2D* m, MultiText* t,
											 const Style* axisStyle, const Style* axisTextStyle,
											 double x, double y, double w, double h, // screen coords
											 bool isVert, // true if the axis is vertical, false is horizontal
											 double minBound, double maxBound, // range in model coordinates
                       double tickInterval, double tickDrawSize, bool showTicks,
											 const std::string& axisTitle,
                       int tickFormatWidth, int tickFormatPrecision,
                       double bottomOffset)
    : Widget2D(m, t, x, y, w, h),
			axisStyle(axisStyle),
			axisTextStyle(axisTextStyle),
      minBound(minBound),
      maxBound(maxBound),
      tickInterval(tickInterval),
      tickDrawSize(tickDrawSize),
      showTicks(showTicks),
      isVert(isVert),
      axisTitle(axisTitle),
			tickColor(axisStyle->fg),
			tickFormat({.width = tickFormatWidth, .precision = tickFormatPrecision}),
      bottomOffset(bottomOffset) {
	axisSize = isVert ? w : h;
   }

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
                     y + w / 2, m->getStyle()->f, axisTitle);
    } else {
      t->addCentered(
          x + w / 2,
          y + h + (bottomOffset += 10 + m->getStyle()->f->getHeight()),
          m->getStyle()->f, axisTitle);
    }
  }
}

LinearAxisWidget::LinearAxisWidget(StyledMultiShape2D* m, MultiText* t,
																	 const Style* axisStyle, const Style* axisTextStyle,
                                   double x, double y, double w, double h, bool isVert)
	: AxisWidget(m, t, axisStyle, axisTextStyle, x, y, w, h, isVert) {}

void LinearAxisWidget::setBounds(double minBound, double maxBound) {
  this->minBound = minBound;
  this->maxBound = maxBound;
	scale = axisSize / (maxBound - minBound);
  this->shift = scale*(-minBound)+shift;
}

void LinearAxisWidget::setTickInterval(double tickInterval) {
  this->tickInterval = tickInterval;
}

void LinearAxisWidget::init() {
	//TODO: scale only computed for horizontal? Seems like a bug
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

LogAxisWidget::LogAxisWidget(StyledMultiShape2D* m, MultiText* t,
														 const Style* axisStyle, const Style* axisTextStyle,
                             double x, double y, double w, double h, bool isVert)
	: AxisWidget(m, t, axisStyle, axisTextStyle, x, y, w, h, isVert) {}

void LogAxisWidget::setBounds(double minBound, double maxBound) {
  this->minBound = minBound;
  this->maxBound = maxBound;
  double base = 1 / log(tickInterval);
  scale = w / abs(log(maxBound) * base - log(minBound) * base);
	shift = scale*(-minBound)+shift + (isVert ? y : x);
	scale *= base;
}

void LogAxisWidget::setTickInterval(double tickInterval) {
  this->tickInterval = tickInterval;
}

void LogAxisWidget::init() {
	//  double base = 1 / log(tickInterval);
	//  double scale = w / abs(log(maxBound) * base - log(minBound) * base);
	const Font* f = m->getStyle()->f;
  bottomOffset = tickDrawSize + f->getHeight() * 1.25;
  m->drawLine(x, y + h, x + w, y + h, axisColor);

  if (isVert) {
    double counter = 1;
    for (double tick = maxBound; tick >= minBound;
         tick /= tickInterval, counter *= tickInterval) {
			//      double draw = y + scale * log(counter) * base;
			double draw = y + scale * log(counter); // scale has now been scaled by base

      if (showTicks)
        m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                    tickColor);

      char fmt[50];
      sprintf(fmt, "%%%d.%dlf", tickFormat.width, tickFormat.precision);

      char thing[50];
      sprintf(thing, fmt, tick);

      t->add(x - 20 - f->getWidth(thing, strlen(thing)),
             draw + f->getHeight() / 2, f, tick,
             tickFormat.width, tickFormat.precision);
    }
  } else {
    double counter = 1;
    for (double tick = minBound; tick <= maxBound;
         tick *= tickInterval, counter *= tickInterval) {
			//      double draw = x + scale * log(counter) * base;
      double draw = x + scale * log(counter);

      if (showTicks)
        m->drawLine(draw, y + h + tickDrawSize, draw, y + h - tickDrawSize,
                    tickColor);

      t->addCentered(draw, y + h + bottomOffset, f, tick,
                     tickFormat.width, tickFormat.precision);
    }
  }

  addAxisTitle();
}

TextAxisWidget::TextAxisWidget(StyledMultiShape2D* m, MultiText* t,
															 const Style* axisStyle, const Style* axisTextStyle,
															 double x, double y, double w, double h, bool isVert)
	: AxisWidget(m, t, axisStyle, axisTextStyle, x, y, w, h, isVert), tickLabels(vector<string>()) {}

void TextAxisWidget::setTickLabels(vector<string> tickLabels) {
  this->tickLabels = tickLabels;
}

void TextAxisWidget::init() {
  minBound = 0;
  maxBound = tickLabels.size() * (tickLabels.size() + 1);
  tickInterval = tickLabels.size();
	const Font* f = m->getStyle()->f;
  bottomOffset = tickDrawSize + f->getHeight() * 1.25;
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
      t->add(x, draw, f, current);

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
      t->addCentered(draw, float(y + h + bottomOffset), f, current);
      index++;
    }
  }
  addAxisTitle();
}

float LinearAxisWidget::transform(double v) const {
	return v * scale + shift;
}

float LogAxisWidget::transform(double v) const {
	return log(v) * scale + shift;
}

float TextAxisWidget::transform(double v) const {
	return v; //TODO: ???
}

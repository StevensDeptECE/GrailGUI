#include "opengl/AxisWidget.hh"

#include <cmath>

void AxisWidget::setBounds(double minBound, double maxBound) {
  this->minBound = minBound;
  this->maxBound = maxBound;
}

void AxisWidget::setTickInterval(double tickInterval) {
  this->tickInterval = tickInterval;
}

void AxisWidget::setTitle(std::string text) { this->axisTitle = text; }

void AxisWidget::setTickStyle(const Style *style) {
  this->axisTickStyle = style;
}

void AxisWidget::setTitleStyle(const Style *style) {
  this->axisTitleStyle = style;
}

void LinearAxisWidget::init() {
  float scale = w / abs(maxBound - minBound);

  float lineWidth = std::min(w, h) * 0.025;

  for (float tick = minBound; tick <= maxBound; tick += tickInterval) {
    float draw = x + scale * tick;
    m->drawLine(draw, y + h + lineWidth, draw, y + h - lineWidth, grail::black);
    t->add(draw, y+h+20, axisTickStyle->f, tick, 2, 2);
  }
}

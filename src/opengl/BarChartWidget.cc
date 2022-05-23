#define _USE_MATH_DEFINES
#include "opengl/BarChartWidget.hh"

#include <algorithm>
#include <cmath>

// For C++ 20 use of numbers::pi
//#include <numbers>

#include "util/Ex.hh"

using namespace std;

void BarChartWidget::setBarWidth(double width) { barWidth = width; }

void BarChartWidget::setValues(const vector<double>& values) {
  this->values = values;
}

void BarChartWidget::setNames(const vector<string>& names) {
  this->names = names;
}

void BarChartWidget::init() {
  xAxis->setTickLabels(names);
  if (values.size() < 1 || names.size() < 1) {
    cerr << "names and values vectors cannot be zero length\n";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }

  if (values.size() != names.size()) {
    cerr << "names and values vectors must be the same length\n";
    throw(Ex1(Errcode::VECTOR_MISMATCHED_LENGTHS));
  }

  StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, &s->barStyle));

  double min = yAxis->getMinBound();
  double max = yAxis->getMaxBound();

  double axisInterval = yAxis->getTickInterval();
  double scale;
  double base;

  if (yAxisType == AxisType::LOGARITHMIC) {
    base = 1 / log(axisInterval);
    scale = -h / abs(log(max) * base - log(min) * base);
    transform(values.begin(), values.end(), values.begin(),
              [=, this](double d) -> double {
                return y + h + scale * log(d) * base;
              });
  } else {
    scale = -h / abs(max - min);
    transform(values.begin(), values.end(), values.begin(),
              [=, this](double d) -> double { return y + h + scale * d; });
  }

  double barCorrection = (yAxisType == AxisType::LINEAR)
                             ? -scale * yAxis->getMinBound()
                             : -scale * log(yAxis->getMinBound()) * base;

  double xscale = w / (values.size() + 1);
  double halfBarWidth = barWidth / 2;

  auto barColor = s->barColors.begin();
  auto outlineColor = s->barOutlineColors.begin();
  for (int i = 1; i < values.size() + 1; i++) {
    double barXLocation = x + xscale * i - halfBarWidth;
    double barYLocation = values[i - 1] + barCorrection;

    m->drawRectangle(barXLocation, barYLocation, barWidth, y + h - barYLocation,
                     *outlineColor);
    m->fillRectangle(barXLocation, barYLocation, barWidth, y + h - barYLocation,
                     *barColor);
    barColor++;
    outlineColor++;
    if (barColor == s->barColors.end()) {
      barColor = s->barColors.begin();
    }
    if (outlineColor == s->barOutlineColors.end()) {
      outlineColor = s->barOutlineColors.begin();
    }
  }

  commonRender();
}

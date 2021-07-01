#include "opengl/BarChartWidget.hh"

#include <algorithm>
#include <numbers>

#include "opengl/AngledMultiText.hh"
#include "util/Ex.hh"

using namespace std;

void BarChartWidget::setBarWidth(double width) { barWidth = width; }

void BarChartWidget::setBarColors(const std::vector<glm::vec4> &colors) {
  barColors = colors;
}
void BarChartWidget::setValues(const vector<double> &values) {
  this->values = values;
}

void BarChartWidget::setNames(const vector<string> &names) {
  this->names = names;
}

void BarChartWidget::createXAxis(AxisType a) {
  xAxisType = a;
  MultiText *tnew = c->addLayer(new MultiText(c, xAxisTextStyle));

  switch (a) {
    case LINEAR: {
      cout << "a bar chart can't have a linear x axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;

    case LOGARITHMIC: {
      cout << "a bar chart can't have a logarithmic x axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;

    case TEXT: {
      xAxis = new TextAxisWidget(m, tnew, x, y, w, h);
      xAxis->setTickLabels(names);
    }; break;
  }
}

void BarChartWidget::createYAxis(AxisType a) {
  yAxisType = a;
  StyledMultiShape2D *rot90 = c->addLayer(
      new StyledMultiShape2D(c, m->getStyle(), numbers::pi / 2, x - w, y + h));
  AngledMultiText *t90 = c->addLayer(
      new AngledMultiText(c, yAxisTextStyle, numbers::pi / 2, x - w, y + h));

  switch (a) {
    case LINEAR: {
      yAxis = new LinearAxisWidget(rot90, t90, 0, 0, w, h);
    }; break;

    case LOGARITHMIC: {
      yAxis = new LogAxisWidget(rot90, t90, 0, 0, w, h);
    }; break;

    case TEXT: {
      cout << "a bar chart can't have a text y axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;
  }
}

void BarChartWidget::init() {
  if (values.size() < 1 || names.size() < 1) {
    cerr << "names and values vectors cannot be zero length\n";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }

  if (values.size() != names.size()) {
    cerr << "names and values vectors must be the same length\n";
    throw(Ex1(Errcode::VECTOR_MISMATCHED_LENGTHS));
  }

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

  auto currentColor = barColors.begin();
  for (int i = 1; i < values.size() + 1; i++) {
    double barXLocation = x + xscale * i - halfBarWidth;
    double barYLocation = values[i - 1] + barCorrection;

    m->fillRectangle(barXLocation, barYLocation, barWidth, y + h - barYLocation,
                     *currentColor);
    currentColor++;
    if (currentColor == barColors.end()) {
      currentColor = barColors.begin();
    }
  }

  commonRender();
}

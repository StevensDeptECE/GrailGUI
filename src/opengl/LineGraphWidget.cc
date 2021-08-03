#define _USE_MATH_DEFINES
#include "opengl/LineGraphWidget.hh"

#include <algorithm>
//#include <numbers> For C++ 20 constants

#include "util/Ex.hh"

using namespace std;

void LineGraphWidget::setDataStyle(const Style *s) { dataStyle = s; }

void LineGraphWidget::setPointFormat(char pt, double size, glm::vec4 &color) {
  pointSize = size;
  markerFunction = map[pt];
  pointColor = color;
}

void LineGraphWidget::setXPoints(const std::vector<double> &xPoints) {
  this->xPoints = xPoints;
}

void LineGraphWidget::setYPoints(const std::vector<double> &yPoints) {
  this->yPoints = yPoints;
}

void LineGraphWidget::createXAxis(AxisType a) {
  xAxisType = a;
  StyledMultiShape2D *mnew = c->addLayer(new StyledMultiShape2D(c, xAxisStyle));
  MultiText *tnew = c->addLayer(new MultiText(c, xAxisTextStyle));

  switch (a) {
    case LINEAR: {
      xAxis = new LinearAxisWidget(mnew, tnew, x, y, w, h);
    }; break;

    case LOGARITHMIC: {
      xAxis = new LogAxisWidget(mnew, tnew, x, y, w, h);
    }; break;

    case TEXT: {
      cout << "a line graph can't have a text axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;
  }
}

void LineGraphWidget::createYAxis(AxisType a) {
  yAxisType = a;
  StyledMultiShape2D *rot90 =
      c->addLayer(new StyledMultiShape2D(c, yAxisStyle, M_PI_2, x - w, y + h));
  MultiText *t90 = c->addLayer(new MultiText(c, yAxisTextStyle, 0, x, y));

  switch (a) {
    case LINEAR: {
      yAxis = new LinearAxisWidget(rot90, t90, 0, 0, h, w);
    }; break;

    case LOGARITHMIC: {
      yAxis = new LogAxisWidget(rot90, t90, 0, 0, h, w);
    }; break;

    case TEXT: {
      cout << "a line graph can't have a text axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;
  }
}

void LineGraphWidget::init() {
  if (xPoints.size() < 1 || yPoints.size() < 1) {
    cerr << "x and y vectors cannot be zero length";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }

  if (xPoints.size() != yPoints.size()) {
    cerr << "x and y vectors must be the same length";
    throw(Ex1(Errcode::VECTOR_MISMATCHED_LENGTHS));
  }

  StyledMultiShape2D *m = c->addLayer(new StyledMultiShape2D(c, dataStyle));

  double xMin = xAxis->getMinBound();
  double xMax = xAxis->getMaxBound();
  double yMin = yAxis->getMinBound();
  double yMax = yAxis->getMaxBound();

  double xInterval = xAxis->getTickInterval();
  double yInterval = yAxis->getTickInterval();

  if (xAxisType == AxisType::LOGARITHMIC) {
    double base = 1 / log(xInterval);
    double scale = w / abs(log(xMax) * base - log(xMin) * base);
    transform(
        xPoints.begin(), xPoints.end(), xPoints.begin(),
        [=, this](double d) -> double { return x + scale * log(d) * base; });
  } else {
    double scale = w / abs(xMax - xMin);
    transform(xPoints.begin(), xPoints.end(), xPoints.begin(),
              [=, this](double d) -> double { return x + scale * d; });
  }

  if (yAxisType == AxisType::LOGARITHMIC) {
    double base = 1 / log(yInterval);
    double scale = -h / abs(log(yMax) * base - log(yMin) * base);
    transform(yPoints.begin(), yPoints.end(), yPoints.begin(),
              [=, this](double d) -> double {
                return y + h + scale * log(d) * base;
              });
  } else {
    double scale = -h / abs(yMax - yMin);
    transform(yPoints.begin(), yPoints.end(), yPoints.begin(),
              [=, this](double d) -> double { return y + h + scale * d; });
  }

  double xPoint1 = xPoints[0];
  double yPoint1 = yPoints[0];

  (m->*markerFunction)(xPoint1, yPoint1, pointSize, pointColor);

  for (int i = 1; i < xPoints.size(); i++) {
    double xPoint2 = xPoints[i];
    double yPoint2 = yPoints[i];

    m->drawLine(xPoint1, yPoint1, xPoint2, yPoint2, grail::black);
    (m->*markerFunction)(xPoint2, yPoint2, pointSize, pointColor);

    xPoint1 = xPoint2;
    yPoint1 = yPoint2;
  }

  commonRender();
}

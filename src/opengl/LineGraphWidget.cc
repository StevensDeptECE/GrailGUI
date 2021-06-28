#include "opengl/LineGraphWidget.hh"

#include <algorithm>

#include "opengl/AngledMultiText.hh"
#include "util/Ex.hh"

using namespace std;

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
  MultiText *tnew = c->addLayer(new MultiText(c, xAxisTextStyle));

  switch (a) {
    case linear: {
      xAxis = new LinearAxisWidget(m, tnew, x, y, w, h);
    }; break;

    case logar: {
      xAxis = new LogAxisWidget(m, tnew, x, y, w, h);
    }; break;
  }
}

void LineGraphWidget::createYAxis(AxisType a) {
  yAxisType = a;

  switch (a) {
    case linear: {
      StyledMultiShape2D *rot90 = c->addLayer(
          new StyledMultiShape2D(c, m->getStyle(), M_PI / 2, x - w, y + h));

      AngledMultiText *t90 = c->addLayer(
          new AngledMultiText(c, yAxisTextStyle, M_PI / 2, x - w, y + h));

      yAxis = new LinearAxisWidget(rot90, t90, 0, 0, w, h);
    }; break;

    case logar: {
      StyledMultiShape2D *rot90 = c->addLayer(
          new StyledMultiShape2D(c, m->getStyle(), M_PI / 2, x - w, y + h));

      AngledMultiText *t90 = c->addLayer(
          new AngledMultiText(c, yAxisTextStyle, M_PI / 2, x - w, y + h));

      yAxis = new LogAxisWidget(rot90, t90, 0, 0, w, h);
    }; break;
  }
}
void LineGraphWidget::setGraphTitle(std::string text) { graphTitle = text; }

void LineGraphWidget::setXAxisTextStyle(const Style *xAxisTextStyle) {
  this->xAxisTextStyle = xAxisTextStyle;
}

void LineGraphWidget::setYAxisTextStyle(const Style *yAxisTextStyle) {
  this->yAxisTextStyle = yAxisTextStyle;
}

void LineGraphWidget::init() {
  if (xPoints.size() < 1 || yPoints.size() < 1) {
    cerr << "x and y vectors cannot be zero length";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }

  double xMin = xAxis->getMinBound();
  double xMax = xAxis->getMaxBound();
  double yMin = yAxis->getMinBound();
  double yMax = yAxis->getMaxBound();

  double xInterval = xAxis->getTickInterval();
  double yInterval = yAxis->getTickInterval();

  if (xAxisType == AxisType::logar) {
    double base = 1 / log(xInterval);
    double scale = w / abs(log(xMax) * base - log(xMin) * base);
    transform(xPoints.begin(), xPoints.end(), xPoints.begin(),
              [=](double d) -> double { return x + scale * log(d) * base; });
  } else {
    double scale = w / abs(xMax - xMin);
    transform(xPoints.begin(), xPoints.end(), xPoints.begin(),
              [=](double d) -> double { return x + scale * d; });
  }

  if (yAxisType == AxisType::logar) {
    double base = 1 / log(yInterval);
    double scale = -h / abs(log(yMax) * base - log(yMin) * base);
    transform(
        yPoints.begin(), yPoints.end(), yPoints.begin(),
        [=](double d) -> double { return y + h + scale * log(d) * base; });
  } else {
    double scale = -h / abs(yMax - yMin);
    transform(yPoints.begin(), yPoints.end(), yPoints.begin(),
              [=](double d) -> double { return y + h + scale * d; });
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

  if (graphTitle.size())
    t->addCentered(x, y - m->getStyle()->f->getHeight(), w,
                   m->getStyle()->f->getHeight() + 10, m->getStyle()->f,
                   graphTitle.c_str(), graphTitle.size());

  m->drawLine(x, y, x + w, y, grail::black);
  m->drawLine(x + w, y, x + w, y + h, grail::black);
  xAxis->init();
  yAxis->init();
}

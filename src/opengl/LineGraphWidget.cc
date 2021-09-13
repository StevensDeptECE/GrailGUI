#include "opengl/LineGraphWidget.hh"

#include <algorithm>
#include <numbers>

#include "util/Ex.hh"

using namespace std;

void LineGraphWidget::setPointFormat(char pt, double size, glm::vec4& color) {
  pointSize = size;
  markerFunction = marker_pointer_table[pt];
}

void LineGraphWidget::setXPoints(const std::vector<double>& xPoints) {
  this->xPoints = xPoints;
}

void LineGraphWidget::setYPoints(const std::vector<double>& yPoints) {
  this->yPoints = yPoints;
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
  commonRender(); // builds the common StyledMultiShape and MultiText, draws axes, bounding box, etc.
	
	//  StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, &s->dataStyle));
  double xMin = xAxis->getMinBound();
  double xMax = xAxis->getMaxBound();
  double yMin = yAxis->getMinBound();
  double yMax = yAxis->getMaxBound();

  double xInterval = xAxis->getTickInterval();
  double yInterval = yAxis->getTickInterval();

  if (xAxisType == LOGARITHMIC) {
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

  if (yAxisType == LOGARITHMIC) {
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

	if (s->drawLines) {
		m->drawPolyline(&xPoints[0], &yPoints[0], xPoints.size(), s->lineColor);
	}

	if (s->drawMarkers) {
		for (int i = 0; i < xPoints.size(); i++) {
			(m->*markerFunction)(xPoints[i], yPoints[i], pointSize, s->pointColor);
		}
	}
}

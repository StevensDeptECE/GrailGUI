#include "opengl/BoxChartWidget.hh"

#include <algorithm>
#include <cmath>
#include <numbers> //For C++20 constants

#include "util/Ex.hh"
#include "xp/Stats.hh"

using namespace std;

void BoxChartWidget::setBoxWidth(double width) { boxWidth = width; }

void BoxChartWidget::setPointsPerBox(int n) { pointsPerBox = n; }

void BoxChartWidget::setData(const vector<double>& data) { this->data = data; }

void BoxChartWidget::setNames(const ::vector<std::string>& names) {
  this->names = names;
}


void BoxChartWidget::init() {
  xAxis->setTickLabels(names);
  if (data.size() < pointsPerBox) {
    cerr << "the data vector must contain at least one data set (minimum "
         << pointsPerBox << " points)\n";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }

  if (names.size() < 1) {
    cerr << "names vectors cannot be zero length\n";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }

  if (data.size() != names.size() * 5) {
    cerr
        << "data vector must contain 5 times the number of elements as names\n";
    throw(Ex1(Errcode::VECTOR_MISMATCHED_LENGTHS));
  }

  StyledMultiShape2D* whiskers =
      c->addLayer(new StyledMultiShape2D(c, &s->whiskerStyle));
  StyledMultiShape2D* boxes = c->addLayer(new StyledMultiShape2D(c, &s->boxStyle));

  double min = yAxis->getMinBound();
  double max = yAxis->getMaxBound();

  double axisInterval = yAxis->getTickInterval();
  double yscale = -h / abs(max - min);
  double xscale = w / (names.size() + 1);
  double barCorrection = -yscale * min;
  double halfBoxWidth = boxWidth / 2;

  auto currentBoxColor = s->boxColors.begin();
  auto currentWhiskerColor = s->whiskerColors.begin();
  auto currentOutlineColor = s->outlineColors.begin();

  for (int i = 0, counter = 1; i < data.size(); i += pointsPerBox, counter++) {
    auto first = data.begin() + i;
    auto last = data.begin() + i + pointsPerBox;
    vector<double> currentBoxData(first, last);

    transform(currentBoxData.begin(), currentBoxData.end(),
              currentBoxData.begin(),
              [=, this](double d) -> double { return y + h + yscale * d; });

    stats::Stats1D<double> dataSummary(currentBoxData);

    double xLocation = x + xscale * counter - halfBoxWidth;
    double yTopLine = dataSummary.five_number_summary().min;
    double yBottomLine = dataSummary.five_number_summary().max;
    double yMedianLine = dataSummary.five_number_summary().median;
    double yBoxTop = dataSummary.five_number_summary().q1;
    double yBoxBottom = dataSummary.five_number_summary().q3;

    // top whisker line
    whiskers->drawLine(xLocation, yTopLine, xLocation + boxWidth, yTopLine,
                       *currentWhiskerColor);
    // bottom whisker line
    whiskers->drawLine(xLocation, yBottomLine, xLocation + boxWidth,
                       yBottomLine, *currentWhiskerColor);

    // median line
    boxes->drawLine(xLocation, yMedianLine, xLocation + boxWidth, yMedianLine,
                    *currentOutlineColor);

    // central lines
    whiskers->drawLine(xLocation + halfBoxWidth, yBottomLine,
                       xLocation + halfBoxWidth, yBoxBottom,
                       *currentWhiskerColor);
    whiskers->drawLine(xLocation + halfBoxWidth, yTopLine,
                       xLocation + halfBoxWidth, yBoxTop, *currentWhiskerColor);

    // rounded rectangle box
    boxes->fillRoundRect(xLocation, yBoxTop, boxWidth, -yBoxTop + yBoxBottom, 5,
                         5, *currentBoxColor);
    boxes->drawRoundRect(xLocation, yBoxTop, boxWidth, -yBoxTop + yBoxBottom, 5,
                         5, *currentOutlineColor);

    currentBoxColor++;
    currentWhiskerColor++;
    currentOutlineColor++;

    if (currentBoxColor == s->boxColors.end()) currentBoxColor = s->boxColors.begin();
    if (currentWhiskerColor == s->whiskerColors.end())
      currentWhiskerColor = s->whiskerColors.begin();
    if (currentOutlineColor == s->outlineColors.end())
      currentOutlineColor = s->outlineColors.begin();
  }

  commonRender();
}

#define _USE_MATH_DEFINES
#include "opengl/BoxChartWidget.hh"

#include <algorithm>
#include <cmath>
//#include <numbers> For C++ 20 constants

#include "util/Ex.hh"
#include "xp/Stats.hh"

using namespace std;

void BoxChartWidget::setWhiskerStyle(const Style* s) { whiskerStyle = s; }

void BoxChartWidget::setBoxStyle(const Style* s) { boxStyle = s; }

void BoxChartWidget::setBoxWidth(double width) { boxWidth = width; }

void BoxChartWidget::setBoxColors(vector<glm::vec4>& colors) {
  boxColors = colors;
}

void BoxChartWidget::setWhiskerColors(vector<glm::vec4>& colors) {
  whiskerColors = colors;
}

void BoxChartWidget::setOutlineColors(vector<glm::vec4>& colors) {
  outlineColors = colors;
}

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
      c->addLayer(new StyledMultiShape2D(c, whiskerStyle));
  StyledMultiShape2D* boxes = c->addLayer(new StyledMultiShape2D(c, boxStyle));

  double min = yAxis->getMinBound();
  double max = yAxis->getMaxBound();

  double axisInterval = yAxis->getTickInterval();
  double yscale = -h / abs(max - min);
  double xscale = w / (names.size() + 1);
  double barCorrection = -yscale * min;
  double halfBoxWidth = boxWidth / 2;

  auto currentBoxColor = boxColors.begin();
  auto currentWhiskerColor = whiskerColors.begin();
  auto currentOutlineColor = outlineColors.begin();

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

    if (currentBoxColor == boxColors.end()) currentBoxColor = boxColors.begin();
    if (currentWhiskerColor == whiskerColors.end())
      currentWhiskerColor = whiskerColors.begin();
    if (currentOutlineColor == outlineColors.end())
      currentOutlineColor = outlineColors.begin();
  }

  commonRender();
}

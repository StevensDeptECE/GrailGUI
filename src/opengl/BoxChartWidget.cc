#include "opengl/BoxChartWidget.hh"

#include <algorithm>
#include <numbers>

#include "opengl/AngledMultiText.hh"
#include "util/Ex.hh"
#include "xp/Stats.hh"

using namespace std;

void BoxChartWidget::setBoxWidth(double width) { boxWidth = width; }

void BoxChartWidget::setBoxColor(glm::vec4 &color) { boxColor = color; }

void BoxChartWidget::setPointsPerBox(int n) { pointsPerBox = n; }

void BoxChartWidget::setData(const std::vector<double> &data) {
  this->data = data;
}

void BoxChartWidget::setNames(const std::vector<std::string> &names) {
  this->names = names;
}

void BoxChartWidget::createXAxis(AxisType a) {
  xAxisType = a;

  MultiText *tnew = c->addLayer(new MultiText(c, xAxisTextStyle));

  switch (a) {
    case LINEAR: {
      cout << "a box chart can't have a linear x axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;

    case LOGARITHMIC: {
      cout << "a box chart can't have a logarithmic x axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;

    case TEXT: {
      xAxis = new TextAxisWidget(m, tnew, x, y, w, h);
      xAxis->setTickLabels(names);
    }; break;
  }
}

void BoxChartWidget::createYAxis(AxisType a) {
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
      cout << "a box chart can't have a logarithmic y axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;

    case TEXT: {
      cout << "a box chart can't have a text y axis\n";
      throw Ex1(Errcode::BAD_ARGUMENT);
    }; break;
  }
}

void BoxChartWidget::init() {
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

  double min = yAxis->getMinBound();
  double max = yAxis->getMaxBound();

  double axisInterval = yAxis->getTickInterval();
  double yscale = -h / abs(max - min);
  double xscale = w / (names.size() + 1);
  double barCorrection = -yscale * min;
  double halfBoxWidth = boxWidth / 2;

  for (int i = 0, counter = 1; i < data.size(); i += pointsPerBox, counter++) {
    auto first = data.begin() + i;
    auto last = data.begin() + i + pointsPerBox;
    vector<double> currentBoxData(first, last);
    // TODO: remove this when stats1d is able to sort itself
    sort(currentBoxData.begin(), currentBoxData.end());
    cout << "Box Number " << i + 1 << " Data: \n";
    for (auto &a : currentBoxData) {
      cout << a << '\n';
    }

    Stats1D<double> untransformedData(&currentBoxData[0], pointsPerBox);
    cout << untransformedData << '\n';

    transform(currentBoxData.begin(), currentBoxData.end(),
              currentBoxData.begin(),
              [=, this](double d) -> double { return y + h + yscale * d; });
    cout << "Transformed Number " << i + 1 << " Data: \n";
    for (auto &a : currentBoxData) {
      cout << a << '\n';
    }

    // TODO: stats1d has a bug where its 5 num summary is reversed from normal,
    // which is actually good for what I want to do here, but not for the rest
    // of statistics, once stats1d is updated, this code will need to get
    // changed
    Stats1D<double> dataSummary(&currentBoxData[0], pointsPerBox);
    cout << dataSummary << '\n';

    double xLocation = x + xscale * counter - halfBoxWidth;
    double yTopLine = dataSummary.getSummary().min;
    double yBottomLine = dataSummary.getSummary().max;
    double yMedianLine = dataSummary.getSummary().median;
    double yBoxTop = dataSummary.getSummary().q1;
    double yBoxBottom = dataSummary.getSummary().q3;

    // top whisker line
    //
    m->drawLine(xLocation, yTopLine, xLocation + boxWidth, yTopLine,
                grail::black);
    // bottom whisker line
    m->drawLine(xLocation, yBottomLine, xLocation + boxWidth, yBottomLine,
                grail::black);

    // median line
    m->drawLine(xLocation, yMedianLine, xLocation + boxWidth, yMedianLine,
                grail::black);

    // central lines
    m->drawLine(xLocation + halfBoxWidth, yBottomLine, xLocation + halfBoxWidth,
                yBoxBottom, grail::black);
    m->drawLine(xLocation + halfBoxWidth, yTopLine, xLocation + halfBoxWidth,
                yBoxTop, grail::black);

    // rounded rectangle box
    m->fillRoundRect(xLocation, yBoxTop, boxWidth, -yBoxTop + yBoxBottom, 5, 5,
                     boxColor);
    m->drawRoundRect(xLocation, yBoxTop, boxWidth, -yBoxTop + yBoxBottom, 5, 5,
                     grail::black);
  }

  commonRender();
}

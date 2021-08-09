#include "opengl/CandlestickChartWidget.hh"

#include <algorithm>
#include <numbers>

#include "util/Ex.hh"

using namespace std;

void CandlestickChartWidget::setLineStyle(const Style* s) { lineStyle = s; }

void CandlestickChartWidget::setBoxStyle(const Style* s) { boxStyle = s; }

void CandlestickChartWidget::setBoxWidth(double width) { boxWidth = width; }

void CandlestickChartWidget::setBoxColors(
    const std::vector<glm::vec4>& colors) {
  boxColors = colors;
}

void CandlestickChartWidget::setBoxOutlineColors(
    const std::vector<glm::vec4>& colors) {
  outlineColors = colors;
}

void CandlestickChartWidget::setData(const vector<double>& data) {
  this->data = data;
}

void CandlestickChartWidget::setNames(const vector<std::string>& names) {
  this->names = names;
}

void CandlestickChartWidget::init() {
  xAxis->setTickLabels(names);
  if (data.size() < 4) {
    cerr << "the data vector must contain at least one data set (minimum 4 "
            "points)\n";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }

  StyledMultiShape2D* lines = c->addLayer(new StyledMultiShape2D(c, lineStyle));
  StyledMultiShape2D* boxes = c->addLayer(new StyledMultiShape2D(c, boxStyle));

  double min = yAxis->getMinBound();
  double max = yAxis->getMaxBound();

  double axisInterval = yAxis->getTickInterval();
  double yscale = -h / abs(max - min);
  double xscale = w / (data.size() / 4 + 1);
  double halfBoxWidth = boxWidth / 2;

  double correction = -yscale * min;

  auto currentBoxColor = boxColors.begin();
  auto currentOutlineColor = outlineColors.begin();

  // for now I will sort the four points given, and assume that it goes low,
  // close, open, high (bearish style)
  // TODO: implement something so that we can have the close be greater than
  // open or vice versa to allow for the green/red bullish/bearish style

  for (int i = 0, counter = 1; i < data.size(); i += 4, counter++) {
    auto first = data.begin() + i;
    auto last = data.begin() + i + 4;
    vector<double> currentBoxData(first, last);

    sort(currentBoxData.begin(), currentBoxData.end());

    transform(currentBoxData.begin(), currentBoxData.end(),
              currentBoxData.begin(),
              [=, this](double d) -> double { return y + h + yscale * d; });

    double xLocation = x + xscale * counter - halfBoxWidth;
    double yTopLine = currentBoxData.at(3) + correction;
    double yBottomLine = currentBoxData.at(0) + correction;
    double yBoxTop = currentBoxData.at(2) + correction;
    double yBoxBottom = currentBoxData.at(1) + correction;
    // cout << "Max: " << yTopLine << "\nOpen: " << yBoxTop
    //      << "\nClose: " << yBoxBottom << "\nMin: " << yBottomLine << "\n";

    // central lines
    lines->drawLine(xLocation + halfBoxWidth, yBottomLine,
                    xLocation + halfBoxWidth, yBoxBottom, grail::black);
    lines->drawLine(xLocation + halfBoxWidth, yTopLine,
                    xLocation + halfBoxWidth, yBoxTop, grail::black);

    // rounded rectangle box
    boxes->fillRectangle(xLocation, yBoxTop, boxWidth, -yBoxTop + yBoxBottom,
                         *currentBoxColor);
    boxes->drawRectangle(xLocation, yBoxTop, boxWidth, -yBoxTop + yBoxBottom,
                         *currentOutlineColor);

    currentBoxColor++;
    currentOutlineColor++;

    if (currentBoxColor == boxColors.end()) currentBoxColor = boxColors.begin();
    if (currentOutlineColor == outlineColors.end())
      currentOutlineColor = outlineColors.begin();
  }

  commonRender();
}

#if 0
void CandlestickChartWidget::chart(const vector<float>& b, int rulerInterval,
                                   int dataPointsPerBar) {
  yAxis->init(min, max, y + h, -h, rulerInterval);

  double wBar = w / (b.size() / dataPointsPerBar +
                     (relativeSpace*  (b.size() / dataPointsPerBar + 1)));
  double spaceBetweenBars = relativeSpace*  wBar;

  for (int start = 0; start < b.size(); start += dataPointsPerBar) {
    float day[dataPointsPerBar];
    for (int i = 0; i < dataPointsPerBar; i++) {
      day[i] = b[start + i];
    }

    double dayMax = day[0];
    for (int i = 1; i < dataPointsPerBar; i++) {
      if (day[i] > dayMax) {
        dayMax = day[i];
      }
    }
    double dayMin = day[0];
    for (int i = 1; i < dataPointsPerBar; i++) {
      if (day[i] < dayMin) {
        dayMin = day[i];
      }
    }

    double xBar = ((start / dataPointsPerBar) + 1)*  spaceBetweenBars +
                  (start / dataPointsPerBar)*  wBar + x;
    double yTopBar = yAxis->transform(dayMax);
    double yBottomBar = yAxis->transform(dayMin);

    m->fillRectangle(xBar, yTopBar, wBar, -yTopBar + yBottomBar, grail::blue);

    double tickS, tickE;

    if (day[0] == dayMin) {
      tickS = -tickThicknessStart;
    } else {
      tickS = tickThicknessStart;
    }

    if (day[dataPointsPerBar - 1] == dayMin) {
      tickE = -tickThicknessEnd;
    } else {
      tickE = tickThicknessEnd;
    }

    m->fillRectangle(xBar, yAxis->transform(day[0]), -spaceBetweenBars / 3,
                     tickS, grail::blue);

    m->fillRectangle(xBar + wBar, yAxis->transform(day[dataPointsPerBar - 1]),
                     spaceBetweenBars / 3, tickE, grail::blue);
  }

  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

  for (float yTick = min; yTick <= max; yTick = yAxis->next(yTick)) {
    float yScreen = yAxis->transform(yTick);
    m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen,
                grail::black);
    t->add(x - 40, yScreen, f, (int)yTick);
  }
}
#endif

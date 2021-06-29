#include "opengl/BoxChartWidget.hh"

#include <algorithm>

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
      new StyledMultiShape2D(c, m->getStyle(), M_PI / 2, x - w, y + h));
  AngledMultiText *t90 = c->addLayer(
      new AngledMultiText(c, yAxisTextStyle, M_PI / 2, x - w, y + h));

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

  vector<Stats1D<double> *> summaries;

  for (int i = 0; i < data.size(); i += pointsPerBox) {
    auto first = data.begin() + i;
    auto last = data.begin() + i + pointsPerBox;
    vector<double> currentBoxData(first, last);
    cout << "Box Number " << i + 1 << " Data: \n";
    for (auto &a : currentBoxData) {
      cout << a << '\n';
    }
    transform(currentBoxData.begin(), currentBoxData.end(),
              currentBoxData.begin(),
              [=](double d) -> double { return y + h + yscale * d; });

    Stats1D<double> dataSummary(&currentBoxData[0], pointsPerBox);
    cout << dataSummary << '\n';
    summaries.push_back(&dataSummary);
  }

  for (int i = 1; i < names.size() + 1; i++) {
    double xLocation = x + xscale * i - halfBoxWidth;
    double yTopLine = summaries[i - 1]->getSummary().max;
    double yBottomLine = summaries[i - 1]->getSummary().min;
    double yMedianLine = summaries[i - 1]->getSummary().median;
    double yBoxTop = summaries[i - 1]->getSummary().q1;
    double yBoxBottom = summaries[i - 1]->getSummary().q3;

    // top whisker line
    m->drawLine(xLocation, yTopLine, xLocation + boxWidth, yTopLine,
                grail::black);
    // bottom whisker line
    m->drawLine(xLocation, yBottomLine, xLocation + boxWidth, yBottomLine,
                grail::black);

    // rounded rectangle box
    m->fillRoundRect(xLocation, yBoxTop, boxWidth, -yBoxTop + yBoxBottom, 5, 5,
                     boxColor);
    m->drawRoundRect(xLocation, yBoxTop, boxWidth, -yBoxTop + yBoxBottom, 5, 5,
                     grail::black);

    m->drawLine(xLocation, yMedianLine, xLocation + boxWidth, yMedianLine,
                grail::black);
  }

  // last things to draw
  if (graphTitle.size())
    t->addCentered(x, y - m->getStyle()->f->getHeight(), w,
                   m->getStyle()->f->getHeight() + 10, m->getStyle()->f,
                   graphTitle.c_str(), graphTitle.size());
  m->drawLine(x, y, x + w, y, grail::black);
  m->drawLine(x + w, y, x + w, y + h, grail::black);
  xAxis->init();
  yAxis->init();
}

#if 0
void BoxChartWidget::chart(const vector<float>& b, int rulerInterval,
                           int dataPointsPerBar,
                           const vector<string>& boxNames) {
  yAxis->init(0, max, y + h, -h, rulerInterval);

  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

  float wBar = w / (b.size() / dataPointsPerBar +
                    (relativeSpace * (b.size() / dataPointsPerBar + 1)));
  float spaceBetweenBars = relativeSpace * wBar;

  for (int start = 0; start < b.size(); start += dataPointsPerBar) {
    float day[dataPointsPerBar];
    for (int i = 0; i < dataPointsPerBar; i++) {
      day[i] = b[start + i];
    }

    int dayMax = day[0];
    for (int i = 1; i < dataPointsPerBar; i++) {
      if (day[i] > dayMax) {
        dayMax = day[i];
      }
    }
    int dayMin = day[0];
    for (int i = 1; i < dataPointsPerBar; i++) {
      if (day[i] < dayMin) {
        dayMin = day[i];
      }
    }

    sort(day, day + dataPointsPerBar);

    float median;
    if (dataPointsPerBar % 2 == 0) {
      median = (day[dataPointsPerBar / 2] + day[dataPointsPerBar / 2 - 1]) / 2;
    } else {
      median = day[dataPointsPerBar / 2];
    }

    int fourth = round((dataPointsPerBar - 1) / 4);

    float per25 = day[fourth];
    float per75 = day[dataPointsPerBar - 1 - fourth];

    float xBar = ((start / dataPointsPerBar) + 1) * spaceBetweenBars +
                 (start / dataPointsPerBar) * wBar + x;
    float yTopLine = yAxis->transform(dayMax);
    float yBottomLine = yAxis->transform(dayMin);
    float medianH = yAxis->transform(median);
    float yTopBox = yAxis->transform(per75);
    float yBottomBox = yAxis->transform(per25);

    // m->fillRectangle(xBar, yTopBar, wBar, - yTopBar + yBottomBar,
    // grail::blue);

    m->fillRoundRect(xBar, yTopBox, wBar, -yTopBox + yBottomBox, 5, 5,
                     grail::blue);
    m->drawRoundRect(xBar, yTopBox, wBar, -yTopBox + yBottomBox, 5, 5,
                     grail::black);

    m->drawLine(xBar + wBar / 2, yTopLine, xBar + wBar / 2, yBottomLine,
                grail::black);
    m->drawLine(xBar, medianH, xBar + wBar, medianH, grail::black);

    m->drawLine(xBar + wBar * (1 - topLineLenght) / 2, yTopLine,
                xBar + wBar * (1 - (1 - topLineLenght) / 2), yTopLine,
                grail::black);
    m->drawLine(xBar + wBar * (1 - topLineLenght) / 2, yBottomLine,
                xBar + wBar * (1 - (1 - topLineLenght) / 2), yBottomLine,
                grail::black);

    t->add(xBar, y + h + 20, f, boxNames[(start / dataPointsPerBar)].c_str(),
           boxNames[(start / dataPointsPerBar)].length());
  }

  for (float yTick = min; yTick <= max; yTick = yAxis->next(yTick)) {
    float yScreen = yAxis->transform(yTick);
    m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen,
                grail::black);
    t->add(x - 40, yScreen, f, (int)yTick);
  }
}
#endif

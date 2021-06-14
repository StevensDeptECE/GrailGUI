#include "opengl/LineGraphWidget.hh"

#include <algorithm>

#include "opengl/AngledMultiText.hh"

using namespace std;

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

#if 0
void LineGraphWidget::legend(float x, float y) {
  const float boxWidth = 150;
  const int fontSize = 12;

  m->drawRectangle(x, y, boxWidth, 15 * colors.size(), grail::black);
  const Font *f = FontFace::get("TIMES", fontSize, FontFace::BOLD);

  for (int i = 0; i < colors.size(); i++) {
    t->add(x + 2, (y + fontSize + ((fontSize + 2) * i)), f, names[i].c_str(),
           names[i].length());
    m->fillRectangle(x + (boxWidth * .75), y + 2 + ((fontSize + 2) * i),
                     boxWidth / 5, fontSize - 1, colors[i]);
  }
}
#endif

void LineGraphWidget::init() {
  if (xPoints.size() < 1 || yPoints.size() < 1) {
    cerr << "x and y vectors cannot be zero length";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
#if 0
      old code that I don't want to delete
void LineGraphWidget::title(const string &s) {
  int fontSize = 12;
  const Font *f = FontFace::get("TIMES", fontSize, FontFace::BOLD);
  t->add(x, y - 10, f, s.c_str(), s.length());
}

void LineGraphWidget::axes(char xLabel[], int xLen, char yLabel[], int yLen, float xMax, float yMax, float xMin, float yMin, float xInterval, float yInterval){
  axes(xLabel,xLen,yLabel,yLen,xMax,yMax,xMin,yMin,xInterval,yInterval,0,xMax);
#endif
  }

#if 0
  void LineGraphWidget::axes(char xLabel[], int xLen, char yLabel[], int yLen,
                             float xMax, float yMax, float xMin, float yMin,
                             float xInterval, float yInterval, int start,
                             int end) {
    startPoint = start;
    endPoint = end;

    int fontSize = 12;
    const Font *f = FontFace::get("TIMES", fontSize, FontFace::BOLD);

    xAxis = new LinearScale();
    yAxis = new LinearScale();

    xAxis->init(xMin, xMax, x, w, xInterval);
    yAxis->init(yMin, yMax, y + h, -h, yInterval);

    for (float yTick = yMin; yTick <= yMax; yTick = yAxis->next(yTick)) {
      float yScreen = yAxis->transform(yTick);
      m->drawLine(x, yScreen, x + 10, yScreen, grail::black);
    }

    for (float xTick = xMin; xTick <= xMax; xTick = xAxis->next(xTick)) {
      int i = 0;
      float xScreen = xAxis->transform(xTick);
      m->drawLine(xScreen, y + h + 5, xScreen, y + h - 5, grail::black);
      t->add(xScreen, y + h + 10, f,
             to_string((int)(xMin + i * xInterval)).c_str(),
             to_string((int)(xMin + i * xInterval)).length());
      i++;
    }
#endif

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

  m->fillCircle(xPoint1, yPoint1, 3.5, 3, grail::blue);

  for (int i = 1; i < xPoints.size(); i++) {
    double xPoint2 = xPoints[i];
    double yPoint2 = yPoints[i];

    m->drawLine(xPoint1, yPoint1, xPoint2, yPoint2, grail::black);
    m->fillCircle(xPoint2, yPoint2, 3.5, 3, grail::blue);

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


#if 0
/*
  curerently assuming that x and y points are sorted
*/
void LineGraphWidget::chart(const vector<float> &xPoints,
                            const vector<float> &yPoints, float xInterval,
                            float yInterval, Scale *xAxis, Scale *yAxis) {
  // TODO: combine with min max element
  float xMax = *max_element(xPoints.begin(), xPoints.end());
  float yMax = *max_element(yPoints.begin(), yPoints.end());
  float xMin = *min_element(xPoints.begin(), xPoints.end());
  float yMin = *min_element(yPoints.begin(), yPoints.end());
#if 0
  t->add(x - (yLen * fontSize * .75), y * 1.5, f, yLabel, yLen);
  t->add(x + (w * .5), y + h + fontSize + 10, f, xLabel, xLen);
#endif
}

  /*
    curerently assuming that x and y points are sorted
  */

  /*
  void LineGraphWidget::chart(float xMax, float yMax, float xInterval, float
  yInterval, Scale *xAxis, Scale *yAxis) { // OLD FUNCTION USE add()

    // float xScale = w / xMax;
    // float yScale = 0.95 * h / yMax;

    xAxis->init(0, xMax, x, w, xInterval);
    yAxis->init(0, yMax, y + h, -h, yInterval);

    // using the transform function probably won't make things any faster
    // and we don't need to keep the transformed vectors after this operation
    //
    // vector<float> xTransformed =
    //     transform(xPoints.begin(), xPoints.end(), xTransformed.begin(), );

    // vector<float> yTransformed =
    //     transform(yPoints.begin(), yPoints.end(), yTransformed.begin(), );

    const Font *f = FontFace::get("TIMES", 12, FontFace::BOLD);

    float xPoint1 = xAxis->transform(xPoints[0]);
    float yPoint1 = yAxis->transform(yPoints[0]);

    m->fillCircle(xPoint1, yPoint1, 3.5, 3, grail::blue);

    for (int i = 1; i < xPoints.size(); i++) {
      float xPoint2 = xAxis->transform(xPoints[i]);
      float yPoint2 = yAxis->transform(yPoints[i]);

      m->drawLine(xPoint1, yPoint1, xPoint2, yPoint2, grail::black);
      m->fillCircle(xPoint2, yPoint2, 3.5, 3, grail::blue);

      xPoint1 = xPoint2;
      yPoint1 = yPoint2;
    }

    for (float yTick = yMin; yTick <= yMax; yTick = yAxis->next(yTick)) {
      float yScreen = yAxis->transform(yTick);
      m->drawLine(x, yScreen, x + 10, yScreen, grail::black);
    }

    for (float xTick = xMin; xTick <= xMax; xTick = xAxis->next(xTick)) {
      float xScreen = xAxis->transform(xTick);
      m->drawLine(xScreen, y + h + 5, xScreen, y + h - 5, grail::black);
    }

    // while (line < yMax / 0.97) {
    //   m->drawLine(x, y + h - lineCount * tickSpace, x + 10,
    //               y + h - lineCount * tickSpace, grail::black);
    //   t->add(x - 40, y + h - lineCount * tickSpace, f, line);
    //   lineCount++;
    //   line += yInterval;
    // }

    // lineCount = 0;
    // line = 0;
    // float xRulerSpace = xScale * xInterval;
    // while (line < xMax) {
    //   m->drawLine(x + lineCount * xRulerSpace, y + h + 5,
    //               x + lineCount * xRulerSpace, y + h - 5, grail::black);
    //   t->add(x + lineCount * xRulerSpace, y + h + 25, f, line);
    //   lineCount++;
    //   line += xInterval;
    // }
  }
  */

  void LineGraphWidget::add(const vector<float> &xPoints,
                            const vector<float> &yPoints, float xInterval,
                            float yInterval, Scale *xAxis, Scale *yAxis,
                            const glm::vec4 &dotColor, string name) {
    colors.push_back(dotColor);
    names.push_back(name);

    float xMax = *max_element(xPoints.begin(), xPoints.end());
    float yMax = *max_element(yPoints.begin(), yPoints.end());
    float xMin = *min_element(xPoints.begin(), xPoints.end());
    float yMin = *min_element(yPoints.begin(), yPoints.end());

    xAxis->init(0, xMax, x, w, xInterval);
    yAxis->init(0, yMax, y + h, -h, yInterval);
                          const vector<float> &yPoints,
                          const glm::vec4 &dotColor, string name) {
  if (xPoints.size() < 1 || yPoints.size() < 1) {
    cerr << "x and y vectors cannot be zero length";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }

  int numPoints = 0;
  if (xPoints.size() > yPoints.size())
    numPoints = yPoints.size();
  else
    numPoints = xPoints.size();

  if (endPoint < numPoints) numPoints = endPoint;

  colors.push_back(dotColor);
  names.push_back(name);
>>>>>>> e66ef4c7e6488ff8f8751c6af0992c5cc49bbdd3

float xPoint1 = xAxis->transform(xPoints[startPoint]);
float yPoint1 = yAxis->transform(yPoints[startPoint]);

m->fillCircle(xPoint1, yPoint1, 3.5, 3, grail::blue);

for (int i = startPoint + 1; i < numPoints; i++) {
  float xPoint2 = xAxis->transform(xPoints[i]);
  float yPoint2 = yAxis->transform(yPoints[i]);

  m->drawLine(xPoint1, yPoint1, xPoint2, yPoint2, grail::black);
  m->fillCircle(xPoint2, yPoint2, 3.5, 3, dotColor);

  xPoint1 = xPoint2;
  yPoint1 = yPoint2;
}
}

#if 0
void LineGraphWidget ::chartLog(const float b[], int size, float space,
                                const string barNames[], int logBase) {
  float wBar;
  float location[size][2];
  float hBar[size];

  float bLog[size];

  for (int i = 0; i < size; i++) {
    bLog[i] = log(b[i]) / log(logBase);
  }

  float max = 0.0;
  for (int i = 0; i < size; i++) {
    if (bLog[i] > max) {
      max = bLog[i];
    }
  }

  float maxReal = 0.0;
  for (int i = 0; i < size; i++) {
    if (b[i] > maxReal) {
      maxReal = b[i];
    }
  }

  float scale = 0.8 * h / max;
  for (int i = 0; i < size; i++) {
    hBar[i] = bLog[i] * scale;
  }

  wBar = w / (size + (space * (size + 1)));
  float realSpace = space * wBar;

  location[0][0] = realSpace + x;
  for (int i = 1; i < size; i++) {
    location[i][0] = (i + 1) * realSpace + i * wBar + x;
  }

  for (int i = 0; i < size; i++) {
    location[i][1] = h - hBar[i] + y;
  }

  for (int i = 0; i < size; i++) {
    m->fillRectangle(location[i][0], location[i][1], wBar, hBar[i],
                     grail::blue);
  }

  const Font *f = FontFace::get("TIMES", 12, FontFace::BOLD);

  int lineCount = 1;
  int line = logBase;
  float rulerSpace = hBar[0] / bLog[0];
  t->add(x - 40, y + h, f, 0);
  while (line < 1.25 * maxReal) {
    m->drawLine(x, y + h - lineCount * rulerSpace, x + 10,
                y + h - lineCount * rulerSpace, grail::black);
    t->add(x - 40, y + h - lineCount * rulerSpace, f, line);
    lineCount++;
    line *= logBase;
  }

  for (int i = 0; i < size; i++) {
    int n = barNames[i].length();
    char char_array[n + 1];
    strcpy(char_array, barNames[i].c_str());
    t->add(location[i][0], y + h + 20, f, char_array, n);
  }
}
#endif
#endif

#include "opengl/LineGraphWidget.hh"

#include <algorithm>
#include <iostream>
#include <string>

#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"

using namespace std;

void LineGraphWidget::init() { m->drawRectangle(x, y, w, h, grail::black); }

void LineGraphWidget::title(const string &s) {
  const Font *f = FontFace::get("TIMES", 12, FontFace::BOLD);
  t->add(x, y - 10, f, s.c_str(), s.length());
}

/*
  curerently assuming that x and y points are sorted
*/
void LineGraphWidget::chart(const vector<float> &xPoints,
                            const vector<float> &yPoints,
                            float xInterval, float yInterval,
                            Scale *xAxis, Scale *yAxis) {
  if (xPoints.size() < 1 || yPoints.size() < 1) {
    cerr << "x and y vectors cannot be zero length";
    throw(Ex1(Errcode::VECTOR_ZERO_LENGTH));
  }
  if (xPoints.size() != yPoints.size()) {
    cerr << "x and y vectors should be of equal lengths \n";
    throw(Ex1(Errcode::VECTOR_MISMATCHED_LENGTHS));
  }

  // TODO: combine with min max element
  float xMax = *max_element(xPoints.begin(), xPoints.end());
  float yMax = *max_element(yPoints.begin(), yPoints.end());
  float xMin = *min_element(xPoints.begin(), xPoints.end());
  float yMin = *min_element(yPoints.begin(), yPoints.end());

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

  m->fillCircle(xPoint1, yPoint1, 3.5, 3,
                grail::blue);

  for (int i = 1; i < xPoints.size(); i++) {
    float xPoint2 = xAxis->transform(xPoints[i]);
    float yPoint2 = yAxis->transform(yPoints[i]);

    m->drawLine(xPoint1, yPoint1, xPoint2, yPoint2,
                grail::black);
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
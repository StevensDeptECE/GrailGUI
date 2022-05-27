#include "opengl/GapMinderWidget.hh"

#include <algorithm>
#include <iostream>
#include <string>

#include "data/GapMinderLoader.hh"
#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"

using namespace std;

void GapMinderWidget::init() { m->drawRectangle(x, y, w, h, grail::black); }

void GapMinderWidget::setTitle(const string& s) {
  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);
  t->add(x, y - 10, f, s.c_str(), s.length());
}

/*

*/
void GapMinderWidget::chart(const vector<float>& yLocations,
                            const vector<float>& xLocations,
                            const vector<float>& sizes, int rulerIntervalX,
                            int rulerIntervalY, const vector<glm::vec4>& c) {
  yAxis->init(minY, maxY, y + h, -h, rulerIntervalY);
  xAxis->init(minX, maxX, x, w, rulerIntervalX);

  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

  float rad[sizes.size()];

  for (int i = 0; i < sizes.size(); i++) {
    rad[i] = sqrt(sizes[i] / 3.14);
  }

  float minRad = rad[0];
  for (int i = 1; i < sizes.size(); i++) {
    if (minRad < rad[i]) {
      minRad = rad[i];
    }
  }

  float radScale = minMultiplier / minRad;

  for (int i = 0; i < sizes.size(); i++) {
    rad[i] *= radScale;
  }

  for (int i = 0; i < xLocations.size(); i++) {
    float xPoint = xAxis->transform(xLocations[i]);
    float yPoint = yAxis->transform(yLocations[i]);

    cout << "hello indide draw" << endl;

    m->fillCircle(xPoint, yPoint, rad[i], 3, c[i]);
    m->drawCircle(xPoint, yPoint, rad[i], 3, grail::black);
  }

  for (float yTick = minY; yTick <= maxY; yTick = yAxis->next(yTick)) {
    float yScreen = yAxis->transform(yTick);
    m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen,
                grail::black);
    t->add(x - 40, yScreen, f, (int)yTick);
  }

  for (float xTick = minX; xTick <= maxX; xTick = xAxis->next(xTick)) {
    float xScreen = xAxis->transform(xTick);
    m->drawLine(xScreen, y + h + 5, xScreen, y + h - 5, grail::black);
    t->add(xScreen - 5, y + h + 20, f, (int)xTick);
  }
}

void GapMinderWidget::loadData(string sy, string sx, string ss, int startY,
                               int endY) {
  gml = new GapMinderLoader("res/GapMinder/GapMinderDBFile");

  d = gml->getDataset(sx.c_str());
  d2 = gml->getDataset(sy.c_str());
  d3 = gml->getDataset(ss.c_str());
  startYear = startY;
  endYear = endY;
}

void GapMinderWidget::animate(int rulerIntervalX, int rulerIntervalY) {
  if (startYear == endYear) {
    return;
  }

  cout << "hello inside animate" << endl;

  // m->clear();
  // t->clear();

  vector<float> x1 = gml->getAllDataOneYear(startYear, d);

  vector<float> x2;

  vector<float> y1 = gml->getAllDataOneYear(startYear, d2);

  vector<float> y2;

  vector<float> s1 = gml->getAllDataOneYear(startYear, d3);

  vector<float> s2;

  vector<glm::vec4> colorContinent = {grail::yellow, grail::blue, grail::cyan,
                                      grail::green,  grail::pink, grail::red};

  vector<glm::vec4> c2;

  for (int i = 0; i < x1.size(); i++) {
    if (x1[i] < 1000000 && y1[i] < 1000000 && s1[i] < 1000000) {
      x2.push_back(x1[i]);
      y2.push_back(y1[i]);
      s2.push_back(s1[i]);
      c2.push_back(colorContinent[gml->continents[i]]);
    }
  }

  chart(y2, x2, s2, rulerIntervalX, rulerIntervalY, c2);

  startYear++;
}

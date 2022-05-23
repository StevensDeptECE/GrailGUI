#include "opengl/SparklineWidget.hh"

#include <algorithm>
#include <iostream>
#include <string>

#include "opengl/MultiText.hh"
#include "opengl/StyledMultiShape2D.hh"

using namespace std;

void SparklineWidget::init() { m->drawRectangle(x, y, w, h, grail::black); }

void SparklineWidget::setTitle(const string& s) {
  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);
  t->add(x, y - 10, f, s.c_str(), s.length());
}

void SparklineWidget::chart(const vector<float>& yLocations,
                            const vector<float>& xLocations, glm::vec4& c) {
  yAxis->init(minY, maxY, y + h, -h, 0);
  xAxis->init(minX, maxX, x, w, 0);

  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

  float xPoint1 = xAxis->transform(xLocations[0]);
  float yPoint1 = yAxis->transform(yLocations[0]);

  for (int i = 1; i < xLocations.size(); i++) {
    float xPoint2 = xAxis->transform(xLocations[i]);
    float yPoint2 = yAxis->transform(yLocations[i]);

    m->drawLine(xPoint1, yPoint1, xPoint2, yPoint2, c);

    xPoint1 = xPoint2;
    yPoint1 = yPoint2;
  }
}
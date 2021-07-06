#include "opengl/GapMinderWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

void GapMinderWidget::init() {
  m ->drawRectangle(x, y, w, h, grail::black);
}

void GapMinderWidget::setTitle(const string& s){
  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);
    t->add(x, y-10, f , s.c_str(), s.length()); 
}

/*

*/
void GapMinderWidget::chart(const vector<float>& yLocations, 
    const vector<float>& xLocations, const vector<float>& sizes, int rulerIntervalX, int rulerIntervalY,
    const vector <glm::vec4>& c){

    yAxis->init(minY, maxY, y+h, -h, rulerIntervalY);
    xAxis->init(minX, maxX, x, w, rulerIntervalX);

    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    float rad[sizes.size()];

    for (int i = 0; i < sizes.size(); i++){
      rad[i] = sqrt(sizes[i]/3.14);
    }

    float minRad = rad[0];
    for (int i = 1; i < sizes.size(); i++){
      if(minRad < rad[i]){
        minRad = rad[i];
      }
    }

    float radScale = minMultiplier/minRad;

    for (int i = 0; i < sizes.size(); i++){
      rad[i] *= radScale;
    }

    for (int i = 0; i < xLocations.size(); i++) {
      float xPoint = xAxis->transform(xLocations[i]);
      float yPoint = yAxis->transform(yLocations[i]);

      m->fillCircle(xPoint, yPoint, rad[i], 3, c[i]);
      m->drawCircle(xPoint, yPoint, rad[i], 3, grail::black);
    }


    for (float yTick = minY; yTick <= maxY; yTick = yAxis->next(yTick)) {
      float yScreen = yAxis->transform(yTick);
      m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen, grail::black);
      t->add(x-40, yScreen,f, (int)yTick);
    }

    for (float xTick = minX; xTick <= maxX; xTick = xAxis->next(xTick)) {
      float xScreen = xAxis->transform(xTick);
      m->drawLine(xScreen, y + h + 5, xScreen, y + h - 5, grail::black);
      t->add(xScreen - 5, y+h+20 ,f, (int)xTick);
    }

  }
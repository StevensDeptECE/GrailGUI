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
  draw the bar chart
  b = vector of heights 

*/
void GapMinderWidget::chart(const vector<float>& yLocations, 
    const vector<float>& xLocations, int rulerInterval){

    yAxis->init(minY, maxY, y+h, -h, rulerInterval);

    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);


    for (float yTick = minY; yTick <= maxY; yTick = yAxis->next(yTick)) {
      float yScreen = yAxis->transform(yTick);
      m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen, grail::black);
      t->add(x-40, yScreen,f, (int)yTick);
    }

  }
#include "opengl/BarChartWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

void BarChartWidget::init() {
  m ->drawRectangle(x, y, w, h, grail::black);
}

void BarChartWidget::setTitle(const string& s){
  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);
    t->add(x, y-10, f , s.c_str(), s.length()); 
}

/*
  draw the bar chart
  b = vector of heights 

*/
void BarChartWidget::chart(const vector<float>& b, 
    const vector<string>& barNames, int rulerInterval){

    yAxis->init(min, max, y+h, -h, rulerInterval);


    float wBar = w/(b.size() + (relativeSpace * (b.size() + 1)));
    float spaceBetweenBars = relativeSpace * wBar;

    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    for (int i = 0; i < b.size(); i++){
      float xBar = (i+1)*spaceBetweenBars + i*wBar + x;
      float yTopBar = yAxis->transform(b[i]);

      m->fillRectangle(xBar, yTopBar, wBar, y+h-yTopBar, grail::blue);
      t->add(xBar, y+h+20, f , barNames[i].c_str(), barNames[i].length());
    }


    for (float yTick = min; yTick <= max; yTick = yAxis->next(yTick)) {
      float yScreen = yAxis->transform(yTick);
      m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen, grail::black);
      t->add(x-40, yScreen,f, (int)yTick);
    }

  }
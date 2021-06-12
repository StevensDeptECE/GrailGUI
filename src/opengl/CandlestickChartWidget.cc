#include "opengl/CandlestickChartWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

void CandlestickChartWidget::init() {
  m ->drawRectangle(x, y, w, h, grail::black);
}

void CandlestickChartWidget::setTitle(const string& s){
  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);
    t->add(x, y-10, f , s.c_str(), s.length()); 
}

/*
  b = vector of heights 

*/
void CandlestickChartWidget::chart(const vector<float>& b, int rulerInterval, int dataPointsPerBar){

    yAxis->init(min, max, y+h, -h, rulerInterval);

    double wBar = w/(b.size()/dataPointsPerBar + (relativeSpace * (b.size()/dataPointsPerBar + 1)));
    double spaceBetweenBars = relativeSpace * wBar;

    for (int start = 0; start < b.size(); start += dataPointsPerBar){
      float day[dataPointsPerBar];
      for (int i = 0; i < dataPointsPerBar; i++){
        day[i] = b[start+i];
      }

      double dayMax = day[0];
      for (int i = 1; i < dataPointsPerBar; i++){
        if (day[i] > dayMax){
          dayMax = day[i];
        }
      }
      double dayMin = day[0];
      for (int i = 1; i < dataPointsPerBar; i++){
        if (day[i] < dayMin){
          dayMin = day[i];
        }
      }

      double xBar = ((start/dataPointsPerBar)+1)*spaceBetweenBars + (start/dataPointsPerBar)*wBar + x;
      double yTopBar = yAxis->transform(dayMax);
      double yBottomBar = yAxis->transform(dayMin);

      m->fillRectangle(xBar, yTopBar, wBar, - yTopBar + yBottomBar, grail::blue);

      double tickS, tickE;

      if (day[0] == dayMin){
       tickS = -tickThicknessStart;
      }else{
        tickS = tickThicknessStart;
      }

      if (day[dataPointsPerBar -1] == dayMin){
       tickE = -tickThicknessEnd;
      }else{
        tickE = tickThicknessEnd;
      }

      m->fillRectangle(xBar, yAxis->transform(day[0]), -spaceBetweenBars/3, 
        tickS, grail::blue);

      m->fillRectangle(xBar + wBar, yAxis->transform(day[dataPointsPerBar-1]), 
        spaceBetweenBars/3, tickE, grail::blue);

    }


    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    for (float yTick = min; yTick <= max; yTick = yAxis->next(yTick)) {
      float yScreen = yAxis->transform(yTick);
      m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen, grail::black);
      t->add(x-40, yScreen,f, (int)yTick);
    }

  }
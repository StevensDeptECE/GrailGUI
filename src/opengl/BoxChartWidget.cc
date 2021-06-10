#include "opengl/BoxChartWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

void BoxChartkWidget::init() {
  m ->drawRectangle(x, y, w, h, grail::black);
}

void BoxChartkWidget::setTitle(const string& s){
  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);
    t->add(x, y-10, f , s.c_str(), s.length()); 
}

/*
  draw the box chart
  b = vector of heights 

*/
void BoxChartkWidget::chart(const vector<float>& b, int rulerInterval, int dataPointsPerBar, const vector<string>& boxNames){

    yAxis->init(0, max, y+h, -h, rulerInterval);

    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    float wBar = w/(b.size()/dataPointsPerBar + (relativeSpace * (b.size()/dataPointsPerBar + 1)));
    float spaceBetweenBars = relativeSpace * wBar;

    for (int start = 0; start < b.size(); start += dataPointsPerBar){
      float day[dataPointsPerBar];
      for (int i = 0; i < dataPointsPerBar; i++){
        day[i] = b[start+i];
      }

      int dayMax = day[0];
      for (int i = 1; i < dataPointsPerBar; i++){
        if (day[i] > dayMax){
          dayMax = day[i];
        }
      }
      int dayMin = day[0];
      for (int i = 1; i < dataPointsPerBar; i++){
        if (day[i] < dayMin){
          dayMin = day[i];
        }
      }

      sort(day, day + dataPointsPerBar);

      float median;
      if (dataPointsPerBar % 2 == 0){
        median = (day[dataPointsPerBar/2] + day[dataPointsPerBar/2 -1])/2;
      }else{
        median = day[dataPointsPerBar/2];
      }

      int fourth = round((dataPointsPerBar-1)/4);

      float per25 = day[fourth];
      float per75 = day[dataPointsPerBar - 1 - fourth];

      float xBar = ((start/dataPointsPerBar)+1)*spaceBetweenBars + (start/dataPointsPerBar)*wBar + x;
      float yTopLine = yAxis->transform(dayMax);
      float yBottomLine = yAxis->transform(dayMin);
      float medianH = yAxis->transform(median);
      float yTopBox = yAxis->transform(per75);
      float yBottomBox = yAxis->transform(per25);


      //m->fillRectangle(xBar, yTopBar, wBar, - yTopBar + yBottomBar, grail::blue);

      m->fillRoundRect(xBar, yTopBox, wBar, -yTopBox + yBottomBox, 5, 5, grail::blue);
      m->drawRoundRect(xBar, yTopBox, wBar, -yTopBox + yBottomBox, 5, 5, grail::black);
      
      m->drawLine(xBar + wBar/2, yTopLine, xBar + wBar/2, yBottomLine, grail::black);
      m->drawLine(xBar, medianH, xBar + wBar, medianH, grail::black);

      m->drawLine(xBar + wBar*(1-topLineLenght)/2, yTopLine, 
        xBar + wBar*(1-(1-topLineLenght)/2), yTopLine, grail::black);
      m->drawLine(xBar + wBar*(1-topLineLenght)/2, yBottomLine, 
        xBar + wBar*(1-(1-topLineLenght)/2), yBottomLine, grail::black);

      t->add(xBar, y+h+20, f , boxNames[(start/dataPointsPerBar)].c_str(), 
        boxNames[(start/dataPointsPerBar)].length());

    }

    for (float yTick = min; yTick <= max; yTick = yAxis->next(yTick)) {
      float yScreen = yAxis->transform(yTick);
      m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen, grail::black);
      t->add(x-40, yScreen,f, (int)yTick);
    }

  }
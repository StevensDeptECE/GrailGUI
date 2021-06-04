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

    yAxis->init(0, max, y+h, -h, rulerInterval);

    float scale = (1/maxMultiplier)*h/max;

    float wBar = w/(b.size() + (relativeSpace * (b.size() + 1)));
    float spaceBetweenBars = relativeSpace * wBar;

    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    for (int i = 0; i < b.size(); i++){
      float xBar = (i+1)*spaceBetweenBars + i*wBar + x;
      float yTopBar = yAxis->transform(b[i]);

      m->fillRectangle(xBar, yTopBar, wBar, y+h-yTopBar, grail::blue);
      t->add(xBar, y+h+20, f , barNames[i].c_str(), barNames[i].length());
    }

    float hBar = b[0] * scale;

    for (float yTick = min; yTick <= max; yTick = yAxis->next(yTick)) {
      float yScreen = yAxis->transform(yTick);
      m->drawLine(tickStart, yScreen, tickStart + tickSize, yScreen, grail::black);
      t->add(x-40, yScreen,f, (int)yTick);
    }

  }

  #if 0
  void BarChartWidget::chartLog(const float b[], int size, float space, const string barNames[], int logBase){
    float wBar;
    float location[size][2];
    float hBar[size];

    float bLog[size];

    for(int i = 0; i < size; i++){
      bLog[i] = log(b[i])/log(logBase);
    }

    float max = 0.0;
    for (int i = 0; i < size; i++){
      if(bLog[i] > max){
        max = bLog[i];
      }
    }

    float maxReal = 0.0;
    for (int i = 0; i < size; i++){
      if(b[i] > maxReal){
        maxReal = b[i];
      }
    }

    float scale = 0.8*h/max;
    for (int i = 0; i < size; i++){
      hBar[i] = bLog[i] * scale;
    }

    wBar = w/(size + (space * (size + 1)));
    float realSpace = space * wBar;

    location[0][0] = realSpace + x;
    for (int i = 1; i < size; i++){
      location[i][0] = (i+1)*realSpace + i*wBar + x;
    }

    for (int i = 0; i < size; i++){
      location[i][1] = h - hBar[i] + y;
    }

    for (int i = 0; i < size; i++){
      m->fillRectangle(location[i][0], location[i][1], wBar, hBar[i], grail::blue);
    }


    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    int lineCount = 1;
    int line = logBase;
    float rulerSpace = hBar[0]/bLog[0]; 
    t->add(x-40, y+h,f, 0);
    while(line < 1.25*maxReal){
      m->drawLine(x, y+h-lineCount*rulerSpace, x+10, y+h-lineCount*rulerSpace, grail::black);
      t->add(x-40, y+h-lineCount*rulerSpace,f, line);
      lineCount++;
      line *= logBase;
    }

    for (int i = 0; i < size; i++){
      int n = barNames[i].length();
      char char_array[n + 1];
      strcpy(char_array, barNames[i].c_str());
      t->add(location[i][0], y+h+20, f , char_array, n); 
    }
  }
  #endif




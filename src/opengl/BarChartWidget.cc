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

void BarChartWidget::title(const string& s){
  const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);
    t->add(x, y-10, f , s.c_str(), s.length()); 
}

/*
  draw the bar chart
  b = array of heights 
  size = size of the array 

*/
void BarChartWidget::chart(const vector<float>& b, float relativeSpace, 
    const vector<string>& barNames, int rulerInterval){

    float max = *max_element(b.begin(), b.end());

    float scale = 0.8*h/max;

    float wBar = w/(b.size() + (relativeSpace * (b.size() + 1)));
    float spaceBetweenBars = relativeSpace * wBar;

    const Font* f = FontFace::get("TIMES", 12, FontFace::BOLD);

    for (int i = 0; i < b.size(); i++){
      float xBar = (i+1)*spaceBetweenBars + i*wBar + x;
      float hBar = b[i] * scale;
      float yTopBar = h - hBar + y;

      m->fillRectangle(xBar, yTopBar, wBar, hBar, grail::blue);
      t->add(xBar, y+h+20, f , barNames[i].c_str(), barNames[i].length());
    }

    float hBar = b[0] * scale;

    int lineCount = 0;
    int line = 0;
    float rulerSpace = hBar*rulerInterval/b[0]; 
    while(line < 1.25*max){
      m->drawLine(x, y+h-lineCount*rulerSpace, x+10, y+h-lineCount*rulerSpace, grail::black);
      t->add(x-40, y+h-lineCount*rulerSpace,f, line);
      lineCount++;
      line += rulerInterval;
    }
    
  }


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




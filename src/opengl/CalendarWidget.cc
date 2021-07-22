#include "opengl/CalendarWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

void CalendarWidget::init() {
  m->drawRectangle(x, y, w, h, color);

  vector <string> weekDays = {"S", "M", "T", "W", "T", "F", "S"};
  vector <string> monthNames = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", 
    "SEP", "OCT", "NOV", "DEC"};

  t->add(x, y - 20, f, monthNames[month-1].c_str(), monthNames[month-1].size());

  float squareW = w/7;
  float squareH = h/7;

  t->add(x+w - 40, y - 20, f, year);
  
  m->drawGrid(x, y, w, h, 7, 7, color);

  for (int i = 0; i < 7; i++){
    t->add(x + i*squareW + squareW*0.4, y + squareH/2, f, weekDays[i].c_str(), weekDays[i].size());
    if (i != 0){
      m->drawLine(x+squareW*i, y, x+squareW*i, y+h, color);
    }
  }

  int column = 0;
  int row = 1;
  for (int i = 0; i < 42; i++){
    if(days[i] != 0){
      t->add(x + squareW*0.4 + column*squareW, y + squareH*0.6 + squareH*row, f, days[i]);
      if (circles[i] != grail::black){
        m->drawCircle(x + squareW/2 + column*squareW, y + squareH/2 + squareH*row, 
            squareH*0.3, 5, circles[i]);
      }
    }
    if(column == 6){
      row++;
      column = 0;
    }else{
      column++;
    }
  }

}

void CalendarWidget::circleDate(int day, glm::vec4 color){
  for (int i = 0; i < 42; i++){
    if(days[i] == day){
      circles[i] = color;
    }
  }
}


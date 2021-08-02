#include "opengl/CalendarWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

void CalendarWidget::init() {
  //sort(events.begin(), events.end());
  yearTitle();
  switch(viewType) {
    case YEARLY:
      drawYear();
      break;
    case MONTHLY:
      drawMonth();
      break;
    case WEEKLY:
      drawWeek();
      break;
    case DAILY:
      break;

    default:
      break;
  }
}

void CalendarWidget::yearTitle(){
  t->add(x+w - 40, y - 20, f, year);
}

void CalendarWidget::internalDrawMonth(float x, float y, float w, float h, int month, const Font* monthNameFont, const Font* dayFont) {
  int days[42];
  JulianDate j(year, month, 1); //TODO: this date not used at all, day of week calculated in next line
  uint32_t weekDay = j.dayOfWeek(year, month, 1); // TODO: this method does not use j at all

  for (int i = 0; i < 42; i++){
    days[i] = 0;
  }

  for (int i = 0; i < 42; i++){
    circles[i] = grail::black;
  }

  uint32_t daysInMonth = JulianDate::daysInMonth[month-1];
  if (month == 2 && j.isLeapYear(year)) {
    daysInMonth++;
  }

  
  for (int i = 1; i <= daysInMonth; i++){
    days[weekDay+i-1] = i;
  }

  t->add(x, y - 12, monthNameFont, JulianDate::monthAbbr[month-1], 3);

  float squareW = w/7;
  float squareH = h/7;
  
  m->drawGrid(x, y, w, h, 7, 7, color);

  for (int i = 0; i < 7; i++){
    t->add(x + i*squareW + squareW*0.4, y + squareH/2, monthNameFont, JulianDate::weekDays[i], 1);
  }

  int column = 0;
  int row = 1;
  //int currentEvent = 0;
  for (int i = 0; i < 42; i++){
    if(days[i] != 0){
      for (int k = 0; k < events.size(); k++){
        if (events[k].month == month && events[k].day == days[i]){
          m->fillEllipse(x + squareW/2 + column*squareW, y + squareH/2 + squareH*row, 
            squareW*0.4, squareH*0.4, 5, events[k].circleColor);
        }
      }
      t->addCentered(x + squareW/2 + column*squareW, y + squareH/2 + squareH*row, dayFont, days[i]);
    }
    if(column == 6){
      row++;
      column = 0;
    }else{
      column++;
    }
  }

}
void CalendarWidget::drawMonth() {
  internalDrawMonth(x, y, w, h, month, monthViewMonthName, monthViewDayNumber);
}

void CalendarWidget::drawYear(){

  cout << "hello in drawYear" << endl;

  float smallW = w/(4+3*yearInBetweenSpace);
  float smallH = h/(3+2*yearInBetweenSpace);
  float spaceSizeW = smallW*yearInBetweenSpace;
  float spaceSizeH = smallH*yearInBetweenSpace;

  for (int i = 0; i< 3; i++){
    for(int j = 0; j < 4; j++){
      //setMonth(year, (j+1)+4*i);
      internalDrawMonth(x + j*(smallW+spaceSizeW), y + i*(smallH+spaceSizeH), smallW, smallH, (j+1)+4*i, yearViewMonthName, yearViewDayNumber);
    }
  }

  yearTitle();
}

void CalendarWidget::drawWeek() {
}


#if 0
void CalendarWidget::circleDate(int day, glm::vec4 colorC){
  for (int i = 0; i < 42; i++){
    if(days[i] == day){
      circles[i] = colorC;
    }
  }
}
#endif

void CalendarWidget::circleDate(int day, int month, glm::vec4 colorC){
  CalendarWidget::event e;
  e.day = day;
  e.month = month; 
  e.circleColor = colorC;

  events.push_back(e);
}


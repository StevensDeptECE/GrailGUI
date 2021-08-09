#include "opengl/CalendarWidget.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/MultiText.hh"
#include <string>
#include <iostream>
#include <algorithm>
#include "Document.hh"

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
  JulianDate j(year, month, 1); //TODO: this date not used at all, day of week calculated in next line
  uint32_t weekDay = j.dayOfWeek(year, month, 1); // TODO: this method does not use j at all


  uint32_t daysInMonth = JulianDate::daysInMonth[month-1];
  if (month == 2 && j.isLeapYear(year)) {
    daysInMonth++;
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
  int day = 1;
  //int currentEvent = 0;
  for (int i = 0; i < 42; i++){
    if(i >= weekDay && day <= daysInMonth){
      for (int k = 0; k < events.size(); k++){
        if (events[k].month == month && events[k].day == day){
          m->fillEllipse(x + squareW/2 + column*squareW, y + squareH/2 + squareH*row, 
            squareW*0.4, squareH*0.4, 5, events[k].circleColor);
        }
      }
      t->addCentered(x + squareW/2 + column*squareW, y + squareH/2 + squareH*row, dayFont, day);
      day++;
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

  t->add(x, y - 12, weekViewMonthName, JulianDate::monthAbbr[month-1], 3);

  float squareW = w/7;
  float squareH = h*weekRatio;

  m->drawGrid(x, y, w, h*weekRatio, 2, 7, color);
  m->drawGrid(x, y+h*weekRatio, w, h*(1-weekRatio), 1, 7, color);

  JulianDate j(year, month, 1);

  uint32_t daysInPreviousMonth = JulianDate::daysInMonth[month-1];
  if (month == 2 && j.isLeapYear(year)) {
    daysInPreviousMonth++;
  }

  uint32_t weekDay = j.dayOfWeek(year, month, dayInWeek);

  int day;
  bool previous = false;
  if (dayInWeek-weekDay >= 0){
    day = dayInWeek - weekDay;
  }else{
    day = daysInPreviousMonth + dayInWeek-weekDay;
    previous = true;
  }

  uint32_t daysInMonth = JulianDate::daysInMonth[month-1];
  if (month == 2 && j.isLeapYear(year)) {
    daysInMonth++;
  }

  for (int i = 0; i < 7; i++){
    t->addCentered(x + i*squareW + squareW/2, y + squareH/4, weekViewMonthName, JulianDate::weekDays[i], 
      JulianDate::weekDaysSizes[i]);
    if(previous){
      daysInMonth = daysInPreviousMonth;
    }
    t->addCentered(x + i*squareW + squareW/2, y + squareH*3/4, weekViewMonthName, day);
    if(day < daysInMonth){
      day++;
    }else{
      day = 1;
    }
  }
}

void CalendarWidget::circleDate(int day, int month, glm::vec4 colorC, string description){
  CalendarWidget::event e;
  e.day = day;
  e.month = month; 
  e.circleColor = colorC;
  e.description = description;

  events.push_back(e);
}


#pragma once
#include <memory>
#include "opengl/Widget2D.hh"
#include "util/JulianDate.hh"
#include <string>
#include <vector>
#include <algorithm>

class Style;
class CalendarWidget : public Widget2D {
private:
  int year;
  int month;
  glm::vec4 color;
  const Font* f;
  int days[42];
  glm::vec4 circles[42];

public:
  CalendarWidget(StyledMultiShape2D* m, MultiText* t,
   float x, float y, float w, float h,
   int year, int month, glm::vec4 color, const Font* f) : 
    Widget2D(m, t, x, y, w, h), year(year), month(month), color(color), f(f){

      JulianDate j(2000, 1, 1); 

      for (int i = 0; i < 42; i++){
        days[i] = 0;
      }

      for (int i = 0; i < 42; i++){
        circles[i] = grail::black;
      }

      //unsigned int weekDay = dayOfWeek(year, month, 1);
      unsigned int weekDay = j.dayOfWeek(year, month, 1);
      bool feb29 = j.isLeapYear(year);

      //bool feb29 = isLeapYear(year);

      std::vector <int> monthSizes = {31, 28+feb29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

      for (int i = 1; i <= monthSizes[month-1]; i++){
        days[weekDay+i-1] = i;
      }
    }

  CalendarWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h, 
    int year, int month) :
    CalendarWidget(m,t, x, y, w, h, year, month, grail:: black, nullptr){
      f = FontFace::get("TIMES", 12, FontFace::BOLD);
    } 

  CalendarWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) :
    CalendarWidget(m,t, x, y, w, h, 2000, 1, grail:: black, nullptr){
      f = FontFace::get("TIMES", 12, FontFace::BOLD);
    } 

  void init() override;
  void circleDate(int day, glm::vec4 color);
};
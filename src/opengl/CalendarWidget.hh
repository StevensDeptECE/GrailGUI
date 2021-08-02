#pragma once
#include <memory>
#include "opengl/Widget2D.hh"
#include "util/JulianDate.hh"
#include <string>
#include <vector>
#include <algorithm>

class Style;
class CalendarWidget : public Widget2D {
  public:
  enum CalendarView {
    YEARLY, MONTHLY, WEEKLY, DAILY
  };
private:
  CalendarView viewType;
  int year;
  int month;
  glm::vec4 color;
  const Font* f;
  float yearInBetweenSpace;
  const Font* yearViewMonthName;
  const Font* yearViewDayNumber;

  const Font* monthViewMonthName;
  const Font* monthViewDayNumber;

  glm::vec4 circles[42];

  struct event{
    int day;
    int month;
    glm::vec4 circleColor;
  };

  std::vector <CalendarWidget::event> events;

public:

  CalendarWidget(StyledMultiShape2D* m, MultiText* t,
   float x, float y, float w, float h,
   int year, int month, glm::vec4 color, const Font* f) : 
    Widget2D(m, t, x, y, w, h), year(year), month(month), color(color), f(f){
      yearInBetweenSpace = 0.2;
      viewType = YEARLY;
      yearViewMonthName = FontFace::get("TIMES", 12, FontFace::BOLD);
      yearViewDayNumber = FontFace::get("TIMES", 12, FontFace::BOLD);
      monthViewMonthName = FontFace::get("TIMES", 24, FontFace::BOLD);
      monthViewDayNumber = FontFace::get("TIMES", 24, FontFace::BOLD);
    }

  CalendarWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h, 
    int year, int month) :
    CalendarWidget(m,t, x, y, w, h, year, month, grail:: black, FontFace::get("TIMES", 12, FontFace::BOLD)){
    } 

  CalendarWidget(StyledMultiShape2D* m, MultiText* t, float x, float y, float w, float h) :
    CalendarWidget(m,t, x, y, w, h, 2000, 1) {
    } 

  void init() override;
  void setViewYearly() { viewType = YEARLY; }
  void setViewMonthly() { viewType = MONTHLY; }
  void setViewWeekly() { viewType = WEEKLY; }
  void setViewDaily() { viewType = DAILY; }

  void yearTitle();
  void internalDrawMonth(float x, float y, float w, float h, int month, const Font* monthNameFont, const Font* dayNumFont);
  void drawYear(); // calls 12 monthly draws
  void drawMonth(); // draw monthly calendar summarizing events with colored ellipse and possibly markers like graphs
  void drawWeek(); // draw a weekly calendar with events
  void drawDay(); // draw a daily calendar with events
  void circleDate(int day, glm::vec4 color);
  void circleDate(int day, int month, glm::vec4 color);
};
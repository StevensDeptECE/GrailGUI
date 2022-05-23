#include "Calendar.hh"

void Calendar::drawWeek() const {}

void Calendar::drawMonth() const {
  const int daysPerWeek = 7;
  mt->add(x0 + w / 2, top, start.getMonthName());
  sms->drawGrid(x0, y0, w, h, daysPerWeek, 5, edgeColor);
  const float down = 20;
  float x = x0, top = y0 + down;
  for (int i = 0; i < daysPerWeek; i++) {
    mt->add(x, top, Date::weekDay[i]);
  }
}

void Calendar::drawYear() const {
#if 0
	Date d = start.firstDayOfMonth();
	for (int r = 0, mon = 0; r < 3; r++) {
		float x = x0;
		for (int c = 0; c < 4; c++) {
		}
	}
#endif
}

void Calendar::draw() const { drawMonth(); }

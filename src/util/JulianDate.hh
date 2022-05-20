#pragma once
#include <assert.h>

#include <cmath>
#include <iostream>

/*
  @author Dov Kruger 2019-06-06

  Represent a date as a double for scientific representation of date accurate
  to the microsecond over thousands of years.

  The convention here is that midnight 00:00:00 starts the day, so a
  JulianDate of 0.5 would be 12:00:00 (noon)

  I have set the epoch for this implementation to J2000 (January 1,
  2000)

  There is a related class Date which just stores the integer
  portion for business purposes.


  reference: Get source code from octave the use epoch J0  Jan 1, 0000
  http://www.agopax.it/Libri_astronomia/pdf/Astronomical%20Algorithms.pdf
  https://dl.acm.org/doi/abs/10.1145/219340.219343

*/
class Date;
class JulianDate {
 public:
  friend class Date;
  static const double epoch;
  static const uint16_t daysUpTo[12];
  static const uint16_t daysInMonth[12];
  static const char* monthAbbr[12];
  static const char* monthNames[12];

 private:
  double date;
#if 0
  uint32_t Z;
  double F;
  uint32_t alpha;
  uint32_t A;
  uint32_t B;
  uint32_t C;
  uint32_t D;
  uint32_t E;
#endif
  static constexpr double invSecondsPerDay = 1.0 / 86400;
  JulianDate(double d) : date(d) {}

 public:
  bool static isLeap(
      uint32_t year) {  // ex: 1900 is NOT a leap year (divisible by 100) 1904
                        // is a leap year 2000 is a leap year (% 400)
    return year % 4 == 0 && year % 100 != 0 || year % 400 == 0;
  }
  JulianDate(int32_t year, uint32_t month, uint32_t day, uint32_t hour = 0,
             uint32_t min = 0, uint32_t second = 0);

  JulianDate operator+(double days) const { return JulianDate(date + days); }
  friend double operator-(JulianDate a, JulianDate b) {
    return a.date - b.date;
  }
  JulianDate operator-(double days) const { return JulianDate(date - days); }
  JulianDate operator+=(double days) { return date += days; }
  JulianDate operator-=(double days) { return date -= days; }
  // TODO: Decide when two dates are equal
  //  Right now 1/80 of a msec
  bool operator==(JulianDate orig) { return abs(date - orig.date) < 0.0000125; }
  operator double() const { return date; }
  // TODO: need to implement extraction of date info
  int32_t getYear() const {
    int64_t ddate = int64_t(date);
    int32_t dy = ddate / 365 + ddate / 4 - ddate / 100 + ddate / 400;
    std::cout << dy;
    std::cout << (dy = ddate / 365.2425);
    return epoch + dy;
    //    uint32_t month = (E < 13.5) ? E - 1 : E - 13;
    //    return (month > 2.5) ? C - 4716 : C - 4715;
  }
  uint32_t getMonth() const {
    double ddate = floor(date);
    int32_t mm = ddate - floor(floor(ddate / 365.2425) * 365.2425);
    return mm;
    //     return (E < 13.5) ? E - 1 : E - 13;
  }
  uint32_t getDay() const {
    double ddate = floor(date);
    int32_t dd = ddate - floor(floor(ddate / 365.2425) * 365.2425);
    return dd;  // TODO: this is completely wrong
  }
  uint32_t getHour() const {
    double frac = date - floor(date);
    return uint32_t(frac * 24);
  }
  uint32_t getMinute() const {
    double frac = date - floor(date);
    return uint32_t(frac * (24 * 60)) % 60;
  }
  double getSecond() const {
    double frac = date - floor(date);
    return uint32_t(frac * (24 * 60 * 60)) % 60;
  }

  // write a Julian date into the string at dest
  void format(char dest[]) {}

  //	void format(MultiText* text, uint32_t style);
  double getJulDate() const { return date; }
  void extract(int32_t* year, uint32_t* mm, uint32_t* dd, uint32_t* hh,
               uint32_t* min, uint32_t* ss) {
    double ddate = floor(date);
    int32_t dy = ddate / 365 + ddate / 4 - ddate / 100 + ddate / 400;
    *year = epoch + dy;
    int32_t month = ddate - floor(floor(ddate / 365.2425) * 365.2425);
    *mm = month;
    *dd = 0;  // TODO: use a single lookup table for both month and day, easy
    double whole = floor(date);
    double frac = date - whole;
    uint32_t h = uint32_t(frac *= 24);
    frac -= h;
    *hh = h;
    uint32_t m = uint32_t(frac *= 60);
    *min = m;
    frac -= m;
    *ss = frac * 60;
  }

  friend std::ostream& operator<<(std::ostream& s, const JulianDate& jd) {
    s << JulianDate::monthAbbr[jd.getMonth() - 1];
    return s;
  }
  friend class Date;
};

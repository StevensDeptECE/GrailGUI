#include "util/Date.hh"

using namespace std;

void testDates(int startYear, int endYear) {
  // start with Jan 1, startYear,   ex. 2000 1 1
  // create a Julian Date
  for (int year = startYear; year <= endYear; year++)
    for (int mon = 1; mon <= 12; mon++)
      for (int day = 1; day <= JulianDate::daysInMonth[mon - 1];
           day++) {  // TODO: also check leapyear April 29!
        Date d(year, mon, day);
        int checkYear = d.getYear();
        int checkMonth = d.getMonth();
        int checkDay = d.getDay();
        if (checkYear != year) {
          cerr << "Error: " << checkYear << " != " << year << '\n';
        }
      }
  for (int year = startYear; year <= endYear; year++) {
    Date d(year, 1, 1);
    cout << "diff " << (d - d) << '\n';
    Date d2(year, 2, 1);
    cout << "diff " << (d2 - d) << '\n';
    Date d3(year, 3, 1);
    cout << "diff " << (d3 - d) << '\n';
  }
}

void testMalformedDates(int32_t startYear, int32_t endYear) {
  for (int year = startYear; year <= endYear; year++) {
    int month_day[] = {0, 1, 1, 32, 2, 29, 3, 32, 4, 31, 5, 32};
    for (int i = 0; i < sizeof(month_day) / sizeof(int); i += 2) {
      try {
        Date d1(year, month_day[i], month_day[i + 1]);
      } catch (...) {
        cerr << "malformed date " << year << "-" << month_day[i] << "-"
             << month_day[i + 1] << '\n';
      }
    }
  }
}

// http://mathforum.org/library/drmath/view/51907.html
int main() {
  // test all dates for self-consistency from 2000 to 2400
  uint32_t startYear = 2000, endYear = 2100;
  testDates(startYear, endYear);
  testMalformedDates(startYear, endYear);
  Date d1(2000, 1, 1);
  Date d2(2000, 1, 2);
  assert(d2 - d1 == 1);
  assert(d2 == d1 + 1);

  Date d3(2000, 2, 28);
  cout << d3 - d1 << '\n';
  Date d4(2000, 2, 29);
}

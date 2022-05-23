#include "util/JulianDate.hh"

using namespace std;

void testJulianDate(int startYear, int endYear) {
  // start with Jan 1, startYear,   ex. 2000 1 1
  // create a Julian Date
  JulianDate jdstart(startYear, 1, 1);
  // for each year
  //  for each month
  //  for each day
  JulianDate jd(year, mon, day);

  int checkYear = jd.getYear();
  int checkMonth = jd.getMonth();
  int checkDay = jd.getDay();

  if (checkYear != year || checkMonth != month || checkDay != day) {
    // print out everythign so we can figure this out!
  }
}

// http://mathforum.org/library/drmath/view/51907.html
int main() {
  /*
      JulianDate d1(2000, 1, 1);
      JulianDate d2(2000, 1, 1, 0, 0, 0);
      assert(d1 == d2);
      JulianDate d3(2000, 1, 2);
      assert(d3 == d1 + 1);

      JulianDate d4(2000, 2, 28);
      JulianDate d5(2000, 2, 29);
      JulianDate d6(2001, 2, 28);
      JulianDate d7(2001, 2, 29);
      JulianDate d8(2001, 3, 1);
      JulianDate d9(2001, 3, 32);
      JulianDate d10(2001, 4, 31);


      try {
          JulianDate d7(2001, 2, 29); // should throw exception
      } catch (const Ex &e) {
          cout << e << '\n';
      }
      try {
          JulianDate d9(2001, 3, 32);
      } catch (const Ex &e) {
          cout << e << '\n';
      }

      try {
          JulianDate d10(2001, 4, 31);
      } catch (const Ex &e) {
          cout << e << '\n';
      }


      JulianDate d12(1900, 1, 1);
      JulianDate d13(1900, 1, 1, 0, 0, 0);
      assert(d1 == d2);
      JulianDate d14(1900, 1, 2);
      assert(d3 == d1 + 1);
  */

  /*
      JulianDate test1(2000, 1, 1, 0, 0, 0);
      JulianDate test2(2000, 2, 29);
      JulianDate test3(2000, 12, 31);
      JulianDate test4(2001, 1, 1);
      JulianDate test5(2005, 1, 1);
      JulianDate test6(2100, 1, 1);
      JulianDate test7(2500, 1, 1);
      JulianDate test8(3456, 7, 8, 9, 10, 11);
      JulianDate test9(1999, 1, 1);
      JulianDate test10(1996, 1, 1);
      JulianDate test11(1900, 1, 1);
      JulianDate test12(1600, 1, 1);
      JulianDate test13(1582, 10, 15);

      cout.precision(20);

      cout << test1.getDate() << '\t' << test1.getYear() << '-' <<
     test1.getMonth() << '-' << test1.getDay() << ", " << test1.getHour() << ':'
     << test1.getMinute() << ':' << test1.getSecond() << '\n'
           << test2.getDate() << '\t' << test2.getYear() << '-' <<
     test2.getMonth() << '-' << test2.getDay() << ", " << test2.getHour() << ':'
     << test2.getMinute() << ':' << test2.getSecond() << '\n'
           << test3.getDate() << '\t' << test3.getYear() << '-' <<
     test3.getMonth() << '-' << test3.getDay() << ", " << test3.getHour() << ':'
     << test3.getMinute() << ':' << test3.getSecond() << '\n'
           << test4.getDate() << '\t' << test4.getYear() << '-' <<
     test4.getMonth() << '-' << test4.getDay() << ", " << test4.getHour() << ':'
     << test4.getMinute() << ':' << test4.getSecond() << '\n'
           << test5.getDate() << '\t' << test5.getYear() << '-' <<
     test5.getMonth() << '-' << test5.getDay() << ", " << test5.getHour() << ':'
     << test5.getMinute() << ':' << test5.getSecond() << '\n'
           << test6.getDate() << '\t' << test6.getYear() << '-' <<
     test6.getMonth() << '-' << test6.getDay() << ", " << test6.getHour() << ':'
     << test6.getMinute() << ':' << test6.getSecond() << '\n'
           << test7.getDate() << '\t' << test7.getYear() << '-' <<
     test7.getMonth() << '-' << test7.getDay() << ", " << test7.getHour() << ':'
     << test7.getMinute() << ':' << test7.getSecond() << '\n'
           << test8.getDate() << '\t' << test8.getYear() << '-' <<
     test8.getMonth() << '-' << test8.getDay() << ", " << test8.getHour() << ':'
     << test8.getMinute() << ':' << test8.getSecond() << '\n'
           << test9.getDate() << '\t' << test9.getYear() << '-' <<
     test9.getMonth() << '-' << test9.getDay() << ", " << test9.getHour() << ':'
     << test9.getMinute() << ':' << test9.getSecond() << '\n'
           << test10.getDate() << '\t' << test10.getYear() << '-' <<
     test10.getMonth() << '-' << test10.getDay() << ", " << test10.getHour() <<
     ':' << test10.getMinute() << ':' << test10.getSecond() << '\n'
           << test11.getDate() << '\t' << test11.getYear() << '-' <<
     test11.getMonth() << '-' << test11.getDay() << ", " << test11.getHour() <<
     ':' << test11.getMinute() << ':' << test11.getSecond() << '\n'
           << test12.getDate() << '\t' << test12.getYear() << '-' <<
     test12.getMonth() << '-' << test12.getDay() << ", " << test12.getHour() <<
     ':' << test12.getMinute() << ':' << test12.getSecond() << '\n'
           << test13.getDate() << '\t' << test13.getYear() << '-' <<
     test13.getMonth() << '-' << test13.getDay() << ", " << test13.getHour() <<
     ':' << test13.getMinute() << ':' << test13.getSecond() << '\n';
  */

  JulianDate test(2345, 6, 7, 8, 9, 10);
  test.format((
      char*)"1\tdd-mmm-yyyy HH:MM:SS\n"
            "2\tdd-mmm-yyyy\n"
            "3\tmm/dd/yy\n"
            "4\tmmm\n"
            "5\tmm\n"
            "6\tmm/dd\n"
            "7\tdd\n"
            "8\tyyyy\n"
            "9\tyy\n"
            "10\tmmmyy\n"
            "11\tHH:MM:SS\n"
            "12\tHH:MM\n"
            "13\tdd/mm\n"
            "14\tdd/mm/yy\n"
            "15\tmmm.dd,yyyy HH:MM:SS\n"
            "16\tmmm.dd,yyyy\n"
            "17\tmm/dd/yyyy\n"
            "18\tdd/mm/yyyy\n"
            "19\tyy/mm/dd\n"
            "20\tyyyy/mm/dd\n"
            "21\tmmmyyyy\n"
            "22\tyyyy-mm-dd\n"
            "23\tyyyy-mm-dd HH:MM:SS\n");

  /*
  JulianDateFormat fmt("yyyy-mm-dd");

  cout << fmt.format(d3) << '\n';

  //  fmt.print(x, y,

  cout << d3 << '\n';*/
}
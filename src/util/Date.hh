#pragma once

#include "util/Ex.hh"
#include "util/JulianDate.hh"
/*
	@author Dov Kruger 2019-06-06

	Represent a date as a 32-bit integer for business purposes
	This representation is smaller than JulianDate and is only intended for
	integer days for scheduling purposes, ie calendars and the like.
	Because the internal date is signed, it can represent dates +/- 2 billion days
	from the Epoch. I am going to set the epoch for both JulianDate and this to
	January 1, 2000.
	So we have a looming issue: the Y5755424 problem.  Start planning now...
 */
class Date {
private:
	int32_t date;
	static Date epoch;
	Date(int32_t d) : date(d) {}
public:
	Date(JulianDate jd) : date(uint32_t(jd)) {}
	operator JulianDate() const {
		return JulianDate(date);
	}
	Date(int32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t min, double second) {
		if (month < 1 || month > 12 || day < 1 || day > daysInMonth[day]) {
			throw Ex1(Errcode::BAD_DATE);
		}
		int32_t dY = year - epochYear;
	  int32_t leapYears = dY / 4 - dY / 100 + dY / 400; // is this true
		date = dY * 365 + leapYears + daysUpTo[month-1] + day;
	}
	Date operator +(int32_t days) const {
		return Date(date + days);
	}
	Date operator -(int32_t days) const {
		return Date(date - days);
	}
	Date operator +=(int32_t days) {
		date += days;
		return *this;
	}
	Date operator -=(int32_t days) {
		date -= days;
		return *this;
	}
	//TODO: implement date extraction math
	int32_t getYear() const {
		return 0;
	}
	uint32_t getMonth() const {
		return 0;
	}
	uint32_t getDay() const {
		return 0;
	}
};

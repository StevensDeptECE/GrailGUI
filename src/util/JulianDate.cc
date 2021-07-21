#include "util/JulianDate.hh"

#include "util/Ex.hh"

const double JulianDate::epoch = 2000;
const char* JulianDate::monthNames[12] = {
    "January", "February", "March",     "April",   "May",      "June",
    "July",    "August",   "September", "October", "November", "December"};
const char* JulianDate::monthAbbr[12] = {"Jan", "Feb", "Mar", "Apr",
                                         "May", "Jun", "Jul", "Aug",
                                         "Sep", "Oct", "Nov", "Dec"};

const uint16_t JulianDate::daysUpTo[12] = {
    0,    // Jan
    31,   // Feb
    59,   // Mar
    90,   // Apr
    120,  // May
    151,  // Jun
    181,  // Jul
    212,  // Aug
    243,  // Sep
    273,  // Oct
    304,  // Nov
    334   // Dec
};
const uint16_t JulianDate::daysInMonth[12] = {
    31,  // Jan
    28,  // Feb
    31,  // Mar
    30,  // Apr
    31,  // May
    30,  // Jun
    31,  // Jul
    31,  // Aug
    30,  // Sep
    31,  // Oct
    30,  // Nov
    31   // Dec
};

// std::regex JulianDate::yyyy("yyyy");
// std::regex JulianDate::yy("yy");
// std::regex JulianDate::mmmm("mmmm");
// std::regex JulianDate::mmm("mmm");
// std::regex JulianDate::mm("mm");
// std::regex JulianDate::dd("dd");
// std::regex JulianDate::HH("HH");
// std::regex JulianDate::MM("MM");
// std::regex JulianDate::SS("SS");
#if 0
uint32_t JulianDate::getYear(double date) {
  uint32_t Z = date + 2451545;
  double F = date + 2451545 - Z;
  uint32_t alpha = int((Z-1867216.25)/36524.25);
  uint32_t A = Z + 1 + alpha - int(alpha/4);
  uint32_t B = A + 1524;
  uint32_t C = int((B-122.1)/365.25);
  uint32_t D = int(365.25 * C);
  uint32_t E = int((B-D)/30.6001);
  uint32_t month = (E < 13.5) ? E - 1 : E - 13;
  return (month > 2.5) ? C - 4716 : C - 4715;
}

uint32_t JulianDate::getMonth(double date){
    uint32_t Z = date + 2451545;
    double F = date + 2451545 - Z;
    uint32_t alpha = int((Z-1867216.25)/36524.25);
    uint32_t A = Z + 1 + alpha - int(alpha/4);
    uint32_t B = A + 1524;
    uint32_t C = int((B-122.1)/365.25);
    uint32_t D = int(365.25 * C);
    uint32_t E = int((B-D)/30.6001);
    return (E < 13.5) ? E - 1 : E - 13;
}

uint32_t JulianDate::getDay(double date){
  uint32_t Z = date + 2451545;
    double F = date + 2451545 - Z;
    uint32_t alpha = int((Z-1867216.25)/36524.25);
    uint32_t A = Z + 1 + alpha - int(alpha/4);
    uint32_t B = A + 1524;
    uint32_t C = int((B-122.1)/365.25);
    uint32_t D = int(365.25 * C);
    uint32_t E = int((B-D)/30.6001);
    return int(B - D - int(30.6001 * E) + F);
}
std::string JulianDate::getMonthName(double date){
  uint32_t month = getMonth(date);
  std::string monthName = monthAbbr[month];
  return monthName;
}
#endif

#if 0
void JulianDate::format(MultiText* text, float x, float y, uint32_t style) {
	uint32_t year, mm, dd, hh, mm, ss;
	extract(year, mm, dd, hh, mm, ss);
	char buf[15];
  switch(style) {
	case 0:
		sprintf(buf, "%4d-%02d-%02d", year, mm, dd); break;
	case 1:
		sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", year, mm, dd, hh, mm, ss);
		break;
	case 2:
		sprintf(buf, "%s %02d %4d", monthname[mm], dd, year); break;
	case 3:
		break;
	default:
		break;
	}
	//	text->addText(x, y, buf);
}
#endif

JulianDate::JulianDate(int32_t year, uint32_t month, uint32_t day,
                       uint32_t hour, uint32_t min, uint32_t second) {
  if (month < 1 || month > 12 || day < 1 ||
      day > daysInMonth[month - 1] &&
          !(month == 2 && day == 29 && isLeap(year))) {
    throw Ex1(Errcode::BAD_DATE);
  }

  int32_t dY = year - epoch;
  int32_t leapYears = dY / 4 - dY / 100 + dY / 400;  // is this true
  int32_t extraDay = isLeap(year) && month > 2;
  date = dY * 365 + leapYears + daysUpTo[month - 1] + int32_t(day) +
         (hour * 3600 + min * 60 + second) * invSecondsPerDay + extraDay - 1;

#if 0
	// These cannot be stored in the object, which must be a single number
	//TODO: These calculations, and testing them, are the worst part of this!
	// This has to be massively tested
	Z = date + 2451545;
	F = date + 2451545 - Z;
	alpha = int((Z-1867216.25)/36524.25);
	A = Z + 1 + alpha - int(alpha/4);
	B = A + 1524;
	C = int((B-122.1)/365.25);
	D = int(365.25 * C);
	E = int((B-D)/30.6001);
#endif
}

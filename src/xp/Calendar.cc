#include "Calendar.hh"

class Calendar : public XDLType("Calendar") {
private:
	std::string name;                // name of this calendar
	Date start;
	uint32_t weekendDays : 7; // 0b1000001
	std::vector<uint16_t> yearStart; // Index into the holiday table for each year in the calendar
	std::vector<uint16_t> holidays;   // offset for each holiday in a given year
									  // example: start Jan 1 2020
									  // yearStart = 0  10
									  // holidays: 0 45 ... july 4 ....  dec.25,  next year starts 0 ...
public:
	Calendar(const char filename[]);
	Calendar(const std::string& name
	void save(const char filename[]) const;
	void setHoliday(Date d);
	void setHoliday(Date from, Date to);
	void setRepeatingHoliday(uint32_t yearStart, uint32_t yearEnd,
													 uint32_t month, uint32_t day);
	bool isHoliday(Date d) const;
	
	Date addBusinessDays(Date d, int32_t delta) const;
	int32_t businessDaysBetween(Date d1, Date d2) const;

	void writeMeta(Buffer& b);
	void write(Buffer& b);
};

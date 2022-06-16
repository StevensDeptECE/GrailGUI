#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>

#include "util/Buffer.hh"
#include "util/DynArray.hh"
#include "util/HashMap.hh"
#include "util/datatype.hh"
#include "xdl/std.hh"
#include "xdl/List.hh"

class Quote {
 public:
  uint32_t date;
  uint32_t open, hi, low, close;
  uint64_t volume;
  static constexpr uint32_t size_ = 28;

 public:
  //static HashMap<uint32_t> symTab;
  Quote() : date(0), open(0), hi(0), low(0), close(0), volume(0) {}
  Quote(const char buf[]) {
    char symbol[32];
    uint32_t mm, dd, yyyy;
    double decOpen, decHi, decLow, decClose;
    uint32_t openInterest;
    char sep;  // store the bogus - and , separators
    sscanf(buf, "%u-%u-%u,%lf,%lf,%lf,%lf,%lu,%u", &yyyy, &mm, &dd, &decHi,
           &decLow, &decOpen, &decClose, &volume, &openInterest);
#if 0
		uint32_t* q = symTab.get(symbol);
		if (q == nullptr) {
			symTab.add(symbol, strlen(symbol), 0);
		}
#endif
    date = yyyy * 10000 + mm * 100 + dd;  // yyyymmdd
    open = decOpen * 10000;               //
    hi = decHi * 10000;
    low = decLow * 10000;
    close = decClose * 10000;
  }
void write(Buffer& out) const {
  out.write(date);
  out.write(open);
  out.write(hi);
  out.write(low);
  out.write(close);
  out.write(volume);
  out.fastCheckSpace(size_);
  }
  /*
    compress 64-bit volume by taking only the most significant 1 byte of the number 
    (accurate to 1/255, better than 1%)
    and shifting it with an exponent, 32 would give billions, we don't need more than 40
    since we currently use 255, that would be 2**255 which is absurd.
  */
  uint16_t approximateVolume() const {
    uint32_t power2 = log2(volume);
    double top3 = volume / (1 << (power2-8)); // about 1/255 of the number
    uint8_t mantissa = top3; // should be a number from 0 to 200 ish?
    uint8_t exponent = power2;
    return (mantissa << 8) | exponent;
  }

  /*
  reconstruct approximate original volume by computing mantissa * 2**exponent
  */
  uint64_t approximateVolume(uint16_t approxVol) const {
    uint8_t mantissa = approxVol >> 8;
    uint8_t exponent = approxVol & 0xFF;
    return mantissa << exponent;
  }

  void read(Buffer& in) {
    in.checkSpace(size());
    date = in._readU32();
    open = in._readU32();
    hi = in._readU32();
    low = in._readU32();
    close = in._readU32();
    volume = in._readU64();
  }

  void writeDelta(const Quote& prev, Buffer& out) const {
    //TODO: it is possible for the delta in price to exceed 16 bits, what then?
    out.write(int16_t(int32_t(open) - int32_t(prev.open)));
    out.write(uint16_t(hi - open));
    out.write(uint16_t(open - low));
    out.write(int16_t(int32_t(close) - int32_t(open)));
    out.write(approximateVolume());
    out.fastCheckSpace(size_);
  }

  void readDelta(const Quote& prev, Buffer& in) {
    // delta quotes are 10 bytes per
    in.checkSpace(10);
    open = prev.open + in._readI16();
    hi = open + in._readU16();
    low = open - in._readU16();
    close = open + in._readI16();
    volume = approximateVolume(in._readU16());
  }


  void writeMeta(Buffer& out) {
    out.write(DataType::STRUCT8);
    out.write("Quote");
    out.write(uint8_t(6));  // number of fields
    out.write(DataType::DATE, "date");
    out.write(DataType::U32, "open");
    out.write(DataType::U32, "hi");
    out.write(DataType::U32, "low");
    out.write(DataType::U32, "close");
    out.write(DataType::U64, "volume");
  }

/*
  TODO: How do you write metadata for something if there is more than one way of writing it out?
  You need to pick which method when you decide. For example, if you write out stock quotes as absolute data, you need writeMeta above.
  If you write them with deltas, you need the following metadata.

  This is here as a placeholder. We currently don't use it.
*/
  void writeDeltaMeta(Buffer& out) {
    out.write(DataType::STRUCT8);
    out.write("Quote");
    out.write(uint8_t(5));  // number of fields
    out.write(DataType::U16, "open");
    out.write(DataType::U16, "hi");
    out.write(DataType::U16, "low");
    out.write(DataType::U16, "close");
    out.write(DataType::U32, "volume");
  }
  // STRUCT 5 Quote 5  DATE 4 date U32 4 open U32 2 hi U32 3 low U32 5 close U64
  // 6 volume
  uint32_t size() const {
    return size_;  // number of non-aligned bytes to represent this object
  }
  DataType getDataType() const { return DataType::STRUCT8; }
  static List<Quote>* loadASCII(const char objName[], const char textFile[]);
  static void convertASCIIToBinary(const char objName[],const char textFile[], const char binFile[]);
  static void sendQuote(const List<Quote>* quotes, uint32_t start, uint32_t end);
  static void sendDeltaQuote(const List<Quote>* quotes, uint32_t start, uint32_t end);
};

/*
  TODO: To efficiently encode stock quotes
  no reason to send date each time, it's predictable
  the price differentials can potentially overflow so there must be some error mechanism, not done yet.
  volume is something that doesn't really matter so it can be approximate

  What people need to know is approximately how many shares were traded, not the exact number. within 1% should be fine.
  Most of the time volume is stable, but sometimes there is huge activity. So
  volume = 100, 100 x 10, 100 x 100, 100 x 1000, ... in other words, tiny floating point
  1 byte = 0...255 (mantissa) and 1 byte exponent
  0..255 x 2**0 = 0..255
  0..255 x 2*10 = 0..2550
  0..255 x 2**20 = 0..2,550,000
  0..255 x 2**30 = 0 TO 2.55 billion-ish
  etc.
*/



/*
  TODO: Map from Quote objects to how they should appear on the screen

  This object is a placeholder. Ideally we want it to be completely generic if that's possible. so  "ScreenTable<Quote>" rather than a dedicated table that has to be written for quotes

  Things I want to be able to do:

  display an object (like a Quote) on the screen as a table

  AAPL
  Date  05/29/2022
  Open  146.22
  Hi    149.89
  Low   146.22
  Close 147.18

  or somethign like that...

  And, for a list of objects, we already made automated rendering but haven't checked back in a while, so we have to see how it looks, but should be able to
  draw one object per row of the table, so

  05/29/2022  146.22    149.89    146.22    147.18
  ...
*/
class QuoteTable {
 private:
  unsigned char* data;
  List<Quote> quotes;

 public:
  QuoteTable(const char filename[]);
  
};

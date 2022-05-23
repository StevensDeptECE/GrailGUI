#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>

#include "util/Buffer.hh"
#include "util/DynArray.hh"
#include "util/HashMap.hh"
#include "util/datatype.hh"
#include "xdl/std.hh"

class Quote : public XDLType {
 public:
  uint32_t date;
  uint32_t open, hi, low, close;
  uint64_t volume;
  static constexpr uint32_t size_ = 28;

 public:
  static HashMap<uint32_t> symTab;
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
  void read(Buffer& in) {
    in.checkSpace(size());
    date = in._readU32();
    open = in._readU32();
    hi = in._readU32();
    low = in._readU32();
    close = in._readU32();
    volume = in._readU64();
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
  // STRUCT 5 Quote 5  DATE 4 date U32 4 open U32 2 hi U32 3 low U32 5 close U64
  // 6 volume
  uint32_t size() const override {
    return size_;  // number of non-aligned bytes to represent this object
  }
  DataType getDataType() const { return DataType::STRUCT8; }
};

class DeltaQuoteNoDate : public Quote {
 public:
  void write(const Quote& prev, Buffer& out) const {
    out.write(int16_t(open - prev.open));
    out.write(int16_t(hi - open));
    out.write(int16_t(low - open));
    out.write(int16_t(close - open));
    out.write(uint32_t(volume - prev.volume));
    out.fastCheckSpace(size_);
  }
  void read(const Quote& prev, Buffer& in) {
    in.checkSpace(size());
    open = prev.open + in._readU16();
    hi = open + in._readU16();
    low = open + in._readU16();
    close = open + in._readU16();
    volume = prev.volume + in._readU32();
  }

  void writeMeta(Buffer& out) {
    out.write(DataType::STRUCT8);
    out.write("Quote");
    out.write(uint8_t(5));  // number of fields
    out.write(DataType::U16, "open");
    out.write(DataType::U16, "hi");
    out.write(DataType::U16, "low");
    out.write(DataType::U16, "close");
    out.write(DataType::U32, "volume");
  }
};
class Table {
 private:
  unsigned char* data;
  const Quote* quotes;

 public:
  Table(const char filename[]);
  static void loadASCII(const char textFile[], const char binFile[]);
};

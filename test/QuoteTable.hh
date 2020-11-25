#pragma once

#include <iostream>
#include <fstream>
#include <cstdint>
#include "util/HashMap.hh"
#include "util/DynArray.hh"
#include "util/datatype.hh"
#include "util/Buffer.hh"

class Quote {
 public:
  uint32_t date;
  uint32_t open, hi, low, close;
  uint64_t volume;

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
    date = yyyy * 10000 + mm * 100 + dd;
    open = decOpen * 10000;
    hi = decHi * 10000;
    low = decLow * 10000;
    close = decClose * 10000;
  }
  void writeMeta(Buffer& out) {
    out.write(DataType::STRUCT8);
    out.write("Quote");
    out.write(uint8_t(5)); // number of fields
    out.write(DataType::DATE, "date");
    out.write(DataType::U32, "open");
    out.write(DataType::U32, "hi");
    out.write(DataType::U32, "low");
    out.write(DataType::U32, "close");
    out.write(DataType::U64, "volume");
  }
};

class Table {
 private:
  unsigned char *data;
  const Quote *quotes;

 public:
  Table(const char filename[]);
  static void loadASCII(const char textFile[], const char binFile[]);
};

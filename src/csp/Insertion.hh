#pragma once

#include "util/datatype.hh"

class Insertion {
 public:
  Insertion() : offset(0), d(UINT4) {}
  Insertion(uint32_t offset, DataType d) : offset(offset), d(d) {}
  const uint32_t offset;
  const DataType d;
};

#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "data/BlockLoader2.hh"

// constexpr float NaN = 0xffffffff;

class GapMinderExtra : public BlockLoader {
 public:
  GapMinderExtra() {}

  std::vector<int> getContinents();
};

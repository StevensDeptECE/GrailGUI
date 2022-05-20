#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "data/BlockLoader2.hh"
#include <cstring>


//constexpr float NaN = 0xffffffff;

class GapMinderExtra : public BlockLoader{
public:

  GapMinderExtra(){}

  std::vector <int> getContinents();
  
};



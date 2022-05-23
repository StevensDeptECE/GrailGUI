#include <cstdlib>
#include <iostream>
#include <string>

#include "data/GapMinderLoader.hh"

int main(int argc, char* argv[]) {
  std::string grail = getenv("GRAIL");
  grail += "/test/res/GapMinder/";

  grail += argc > 1 ? (argv[1]) : "GapMinderDBFile";
  for (int i = 0; i < 1000; i++) {
    try {
      GapMinderLoader gml(grail.c_str());
    } catch (const char msg[]) {
      std::cout << msg << std::endl;
    }
  }
}

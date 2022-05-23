#include <stdlib.h>

#include <string>

#include "data/GapMinderBinaryDB.hh"

int main(int argc, char* argv[]) {
  std::string grail = getenv("GRAIL");
  grail += "/test/res/GapMinder/";

  grail += argc > 1 ? (argv[1]) : "";
  GapMinderBinaryDB db(grail.c_str());
}

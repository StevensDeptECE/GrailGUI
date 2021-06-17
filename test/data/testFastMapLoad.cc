#include <iostream>
#include <string>

#include "data/BlockMapLoader.hh"
using namespace std;

int main(int argc, char* argv[]) {
  string grail = getenv("GRAIL");
  grail += "/res/maps/";
  const char* shapefilename = argc > 1 ? argv[1] : "uscounties.bml";
  grail += shapefilename;

  BlockMapLoader bml(grail.c_str());
  // cout << bml.sum() << '\n';
  bml.deltaEncode();
  bml.save("uscountiesdelta.bml");
}

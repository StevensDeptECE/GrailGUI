#include <iostream>

#include "opengl/BlockMapLoader.hh"
using namespace std;

int main(int argc, char* argv[]) {
  const char* shapefilename = argc > 1 ? argv[1] : "uscounties.bml";

  BlockMapLoader bml(shapefilename);
  // cout << bml.sum() << '\n';
  bml.deltaEncode();
  bml.save("uscountiesdelta.bml");
}

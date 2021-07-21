#include "DrawSolarSystem.hh"

int main(int argc, char* argv[]) {
  return GLWin::init(
      new DrawSolarSystem(new NASAEphemerisSolarSystem("SolarSystem.txt")),
      1000, 1000);
}

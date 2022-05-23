#include "DrawSolarSystem.hh"

int main(int argc, char* argv[]) {
  return GLWin::init(new DrawSolarSystem(new SolarSystem("SolarSystem.txt")),
                     1000, 1000);
}

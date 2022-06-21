#include "opengl/Canvas.hh"
#include "opengl/Style.hh"
#include "maps/MapView2D.hh"
#include "util/BLHashMap.hh"

class BlockMapLoader;

int main() {
  Canvas* c = nullptr;
  const Style* s = nullptr;
  BlockMapLoader* bml = nullptr;
  BLHashMap<MapEntry>* bdl = nullptr;
  MapView2D* m = new MapView2D(c, s, bml, bdl);

  delete m;
}
#pragma once

#include <glm/glm.hpp>

//#include "data/BlockMapLoader2.hh"
#include "opengl/MultiShape2D.hh"
#include "opengl/MultiText.hh"
#include "maps/MapNames.hh"
#include "util/BLHashMap.hh"

class BlockMapLoader;
class MapViewer;
class Style;
class MapView2D : public Shape {
 private:
  const Style* style;
  //float centerX, centerY, shiftX, shiftY, scaleX, scaleY;
  //glm::mat4 originalTransform;
  //glm::mat4 transform;
  // pointer to map loader object has advantage of opaqueness in header file
  // and we can have a null map and draw nothing, and change maps
  BlockMapLoader* bml;
  BLHashMap<MapEntry>* bdl;
  uint32_t numIndicesToDraw;
  MultiText* mt;

 public:
  MapView2D(MapViewer* parent, const Style* s, MultiText* mt,
   BlockMapLoader* bml = nullptr, BLHashMap<MapEntry>* bdl = nullptr);
  ~MapView2D();
  MapView2D(const MapView2D& orig) = delete;
  MapView2D& operator= (const MapView2D& orig) = delete;
  void init() override;
  void render(glm::mat4& trans) override;
  void update() override;
  void dump();
};

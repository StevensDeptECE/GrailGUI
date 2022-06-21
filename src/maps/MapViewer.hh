#pragma once

#include <glm/glm.hpp>

#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"
#include "maps/MapNames.hh"

class MultiText;
class MapView2D;
class BlockMapLoader;
template<>
class BLHashMap<MapEntry>;

class MapViewer : public Canvas {
 private:
  float centerLon, centerLat, shiftLon, shiftLat, scaleLon, scaleLat;
  MapView2D* mv;
  MultiText* mt;

 public:
  MapViewer(GLWin* w, Tab* tab, const Style* style, uint32_t vpX, uint32_t vpY,
         uint32_t vpW, uint32_t vpH, uint32_t pX,
         uint32_t pY, BlockMapLoader* bml = nullptr,
            BLHashMap<MapEntry>* bdl = nullptr);

  void zoomIn(float factor);
  void zoomOut(float factor);
  void zoomIn(float lat, float lon, float factor);
  void zoomOut(float lat, float lon, float factor);
  void translatePercent(float percentLon, float percentLat) {
    centerLon += percentLon * scaleLon;
    centerLat += percentLat * scaleLat;
  }
  void translate(float deltaLat, float deltaLon);
  void resetToOriginal();
  void setView();
  void setView(float centerLon, centerLat, float zoom) {
    this->centerLon = centerLong, this->centerLat = centerLat;
    scaleLon = scaleLat = zoom;
  }

};
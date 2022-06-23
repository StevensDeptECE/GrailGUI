#pragma once

#include <glm/glm.hpp>

#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"
#include "maps/MapNames.hh"

class MultiText;

#include "util/BLHashMap.hh"
class MapView2D;
//class BlockMapLoader;
//template<>
//class BLHashMap<MapEntry>;

class MapViewer : public Canvas {
 private:
  float centerLat, centerLon, scaleLat, scaleLon, shiftLat, shiftLon;
  float origCenterLat, origCenterLon, origScaleLat, origScaleLon, origShiftLat, origShiftLon;
  MultiText* mt; //WARNING: mt MUST COME BEFORE mv because it is used in it!!!
  // not great style, but C++ does not allow us an easy way out
  MapView2D* mv;

 public:
  MapViewer(GLWin* w, Tab* tab, const Style* style, uint32_t vpX, uint32_t vpY,
         uint32_t vpW, uint32_t vpH, uint32_t pX,
         uint32_t pY, BlockMapLoader* bml = nullptr,
            BLHashMap<MapEntry>* bdl = nullptr);
  MapViewer(GLWin* w, Tab* tab, const Style* style, 
  BlockMapLoader* bml = nullptr, BLHashMap<MapEntry>* bdl = nullptr) 
  : MapViewer(w, tab, style, 0, 0, w->getWidth(), w->getHeight(), w->getWidth(), w->getHeight(), bml, bdl) {}
  ~MapViewer();
  void setView();
  void zoomIn(float factor);
  void zoomOut(float factor);
  void zoomIn(float lat, float lon, float factor);
  void zoomOut(float lat, float lon, float factor);
  void translatePercent(float percentLat, float percentLon);
  void translate(float deltaLat, float deltaLon);
  void resetToOriginal();
  void setOriginalCoords(float centerLat, float centerLon, float scaleLat, float scaleLon, float shiftLon, float shiftLat);
  void setOrigBounds(float minLat, float maxLat, float minLon, float maxLon);
};

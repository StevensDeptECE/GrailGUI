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
  float centerLat, centerLon, shiftLat, shiftLon, scaleLat, scaleLon;
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
  void zoomIn(float factor);
  void zoomOut(float factor);
  void zoomIn(float lat, float lon, float factor);
  void zoomOut(float lat, float lon, float factor);
  void translatePercent(float percentLat, float percentLon) {
    centerLat += percentLat * scaleLat;
    centerLon += percentLon * scaleLon;
  }
  void translate(float deltaLat, float deltaLon);
  void resetToOriginal();
  void setView();
  void setView(float centerLat, float centerLon, float zoom) {
    this->centerLat = centerLat, this->centerLon = centerLon;
    scaleLat = scaleLon = zoom;
  }
  void setBounds(float minLat, float maxLat, float minLon, float maxLon) {
    centerLon = (minLon + maxLon) * 0.5;
    centerLat = (minLat + maxLat) * 0.5;
    /* OpenGL by default scale -1 to +1 --> projection   --> 0..w-1, 0..h-1  w = 1024
        512  --> -512 ... + 511 + 512
        512  0   0  512
        0    512 0  512
        0    0   0  0
        0    0   0  1

        -180 .. +180  ->  *1/180
        1/180   0   0    0
        0      1/45 0    -1           +1/45*y - 1 -->  -1 .. +1
        0      0    0    0
        0      0    0    1
    */
    scaleLon = 1.0/360, scaleLat = 1.0/90;
  }

};
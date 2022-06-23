#include "maps/MapViewer.hh"

#include <cmath>
#include "data/BlockMapLoader2.hh"
#include "opengl/MultiText.hh"
#include "maps/MapView2D.hh"
#include "opengl/Style.hh"
#include "util/BLHashMap.hh"

MapViewer::MapViewer(GLWin* w, Tab* tab, const Style* style, uint32_t vpX, uint32_t vpY,
         uint32_t vpW, uint32_t vpH,
         uint32_t pX, uint32_t pY,
         BlockMapLoader* bml, BLHashMap<MapEntry>* bdl)
    : Canvas(w, tab, style, vpX, vpY, vpW, vpH, pX, pY) {

    // NOTE: mt MUST be created first becuase mv needs it
    mt = new MultiText(this, style, 12);

    mv = new MapView2D(this, style, mt, bml, bdl);
    // MapView2D automatically sets bounds on this object (the MapViewer)

    //addLayer(mv);  // add the MapView2D to this map FIRST because it is under the text
    //addLayer(mt);  // add the text to this map (on top of the MapView2D)

}

// mv and mt are freed by the Canvas
MapViewer::~MapViewer() {}

void MapViewer::init() {
  mv->init();
  mt->init();
}

void MapViewer::render() {
  Canvas::render();
  mv->render(trans);
  glm::mat4 textTrans = glm::scale(trans, glm::vec3(0.1, -0.1, 1));
  mt->render(textTrans);
}

void MapViewer::setView() {
  setOrthoProjection(centerLon - scaleLon + shiftLon, centerLon + scaleLon + shiftLon, centerLat - scaleLat + shiftLat,
                     centerLat + scaleLat + shiftLat);
  getWin()->setRender();
}

void MapViewer::zoomIn(float factor) {
  scaleLon /= factor; // zoom in by requested factor in both x and y
  scaleLat /= factor;
  setView();
}

void MapViewer::zoomOut(float factor) {
  zoomIn(1/factor);
}

void MapViewer::zoomIn(float lat, float lon, float factor) {
  centerLon = lon, centerLat = lat; // center on desired point
  scaleLon /= factor; // zoom in by requested factor in both x and y
  scaleLat /= factor;
  setView();
}
void MapViewer::zoomOut(float lat, float lon, float factor) {
  zoomIn(lat, lon, 1/factor); // zoom out by the inverse of zoomIn?
  setView();
}

void MapViewer::translatePercent(float percentLat, float percentLon) {
  shiftLat += percentLat * scaleLat, shiftLon += percentLon * scaleLon;
  setView();
}

void MapViewer::translate(float deltaLat, float deltaLon) {
  shiftLat += deltaLat, shiftLon += deltaLon;
  setView();
}

void MapViewer::resetToOriginal() {
  centerLat = origCenterLat, centerLon = origCenterLon, scaleLat = origScaleLat, scaleLon = origScaleLon, shiftLat = origShiftLat, shiftLon = origShiftLon;
  setView();
}

void MapViewer::setOrigBounds(float minLat, float maxLat, float minLon, float maxLon) {
  origCenterLat = (minLat + maxLat) * 0.5;
  origCenterLon = (minLon + maxLon) * 0.5;
  /* 
  OpenGL by default scale -1 to +1 --> projection   --> 0..w-1, 0..h-1  w = 1024
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
  origScaleLat = (std::abs(minLat)+std::abs(maxLat))/4;
  origScaleLon = (std::abs(minLon)+std::abs(maxLon))/4;
  origShiftLat = 0, origShiftLon = 0;
  resetToOriginal();
}

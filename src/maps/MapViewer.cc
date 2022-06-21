#include "maps/MapViewer.hh"

#include "data/BlockMapLoader2.hh"
#include "opengl/MultiText.hh"
#include "maps/MapView2D.hh"
#include "opengl/Style.hh"
#include "util/BLHashMap.hh"

MapViewer::MapViewer(const Style* s, BlockMapLoader* bml = nullptr,
                     BLHashMap<MapEntry>* bdl = nullptr)
    : Canvas(s, ),
    mv(new MapView2D(this, s, bml, bdl)), mt(new MultiText(this, s, 12)) {
  addLayer(mv);  // add the MapView2D to this map
  addLayer(mt);  // add the text to this map (on top of the MapView2D)
//    this->centerX = -74, this->centerY = 40;
//    this->scaleX = 70 / 2, this->scaleY = 70 / 2;

  setView();     // set the projection and call for a redraw
}

void MapViewer::setView() {
  setOrthoProjection(centerLon - scaleLon, centerLon + scaleLon, centerLat - scaleLat,
                     centerLat + scaleLat);
  getWin()->setRender();
}

void MapViewer::zoomIn(float factor) {
  scaleLon *= factor; // zoom in by requested factor in both x and y
  scaleLat *= factor;
  setView();
}

void MapViewer::zoomOut(float factor) {
  zoomIn(1/factor);
}

void MapViewer::zoomIn(float lat, float lon, float factor) {
  centerLon = lon, centerLat = lat; // center on desired point
  scaleLon *= factor; // zoom in by requested factor in both x and y
  scaleLat *= factor;
  setView();
}
void MapViewer::zoomOut(float lat, float lon, float factor) {
  zoomIn(lat, lon 1/factor); // zoom out by the inverse of zoomIn?
  setView();
}

void MapViewer::pan(float deltaLat, float deltaLon) {
  shiftLat += deltaLat, shiftLon += deltaLon;
  setView();
}

void MapViewer::resetToOriginal() {
  
}
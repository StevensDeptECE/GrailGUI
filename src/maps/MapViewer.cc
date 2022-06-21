#include "maps/MapViewer.hh"

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

    addLayer(mv);  // add the MapView2D to this map FIRST because it is under the text
    addLayer(mt);  // add the text to this map (on top of the MapView2D)
    //    this->centerX = -74, this->centerY = 40;
    //    this->scaleX = 70 / 2, this->scaleY = 70 / 2;
}

MapViewer::~MapViewer() {
  delete mv;
  delete mt;
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
  zoomIn(lat, lon, 1/factor); // zoom out by the inverse of zoomIn?
  setView();
}

void MapViewer::translate(float deltaLat, float deltaLon) {
  shiftLat += deltaLat, shiftLon += deltaLon;
  setView();
}

void MapViewer::resetToOriginal() {
  
}
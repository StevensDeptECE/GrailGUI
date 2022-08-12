#pragma once

#include <glm/glm.hpp>

#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"
#include "data/BlockMapLoader.hh"
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
  MultiText* mtCounties; 
  MultiText* mtStates; //WARNING: mt MUST COME BEFORE mv because it is used in it!!!
  // not great style, but C++ does not allow us an easy way out
  MapView2D* mv;
  float countyTextScale, stateTextScale;
  bool displayText, displayCountyNames, displayStateNames, displayOutline, displayFill;

 public:
  MapViewer(GLWin* w, Tab* tab, const Style* style, uint32_t vpX, uint32_t vpY,
         uint32_t vpW, uint32_t vpH, uint32_t pX,
         uint32_t pY, BlockMapLoader* bml = nullptr,
            BLHashMap<MapEntry>* bdl = nullptr, float textScale = 1);
  MapViewer(GLWin* w, Tab* tab, const Style* style,
  BlockMapLoader* bml = nullptr, BLHashMap<MapEntry>* bdl = nullptr, float textScale = 1) 
  : MapViewer(w, tab, style, 0, 0, w->getWidth(), w->getHeight(), w->getWidth(), w->getHeight(), bml, bdl, textScale) {}
  ~MapViewer();
  void init();
  void render();
  void setView();
  void zoomIn(float factor);
  void zoomOut(float factor);
  void zoomInOnCenter(float lat, float lon, float factor);
  void zoomOutOnCenter(float lat, float lon, float factor);
  void translatePercent(float percentLat, float percentLon);
  void translate(float deltaLat, float deltaLon);
  void resetToOriginal();
  void setOriginalCoords(float centerLat, float centerLon, float scaleLat, float scaleLon, float shiftLon, float shiftLat);
  void setOrigBounds(float minLat, float maxLat, float minLon, float maxLon);
  void setTextScale(float countyTextScale, float stateTextScale);
  void increaseTextSize(float factor);
  void decreaseTextSize(float factor);
  void toggleDisplayText();
  void toggleDisplayCountyNames();
  void toggleDisplayStateNames();
  void toggleDisplayOutline();
  void toggleDisplayFill();
  void zoomInOnMouse(float factor);
  void zoomOutOnMouse(float factor);
  void incSegment();
  void decSegment();
  void incNumSegments();
  void decNumSegments();
  void displayAllSegments();
  void displayFirstSegment();
  void displayState(const char stateName[]);
  bool getDisplayCountyNames() const { return displayCountyNames; }
  bool getDisplayStateNames() const { return displayStateNames; }
  bool getDisplayOutline() const { return displayOutline; }
  bool getDisplayFill() const { return displayFill; }
};

#include "maps/MapViewer.hh"

#include <cmath>
#include "data/BlockMapLoader.hh"
#include "opengl/MultiText.hh"
#include "maps/MapView2D.hh"
#include "opengl/Style.hh"
#include "util/BLHashMap.hh"
#include "opengl/Tab.hh"

MapViewer::MapViewer(GLWin* w, Tab* tab, const Style* style, uint32_t vpX, uint32_t vpY,
         uint32_t vpW, uint32_t vpH,
         uint32_t pX, uint32_t pY,
         BlockMapLoader* bml, BLHashMap<MapEntry>* bdl, float textScale)
    : Canvas(w, tab, style, vpX, vpY, vpW, vpH, pX, pY) {

    // NOTE: mt MUST be created first becuase mv needs it
    this->textScale = textScale;
    displayText = true, displayOutline = true, displayFill = true;
    mt = new MultiText(this, style, 12);
    mv = new MapView2D(this, style, mt, bml, bdl, 1/textScale);
    tab->addCanvas(this); // register ourselves in the tab
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
  if (displayText) {
    // scale x and y coordinates by 1/factor in MapView2D
    glm::mat4 textTrans = glm::scale(trans, glm::vec3(textScale, -textScale, 1));
    mt->render(textTrans);
  }
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

void MapViewer::zoomInOnCenter(float lat, float lon, float factor) {
  centerLon = lon, centerLat = lat; // center on desired point
  scaleLon /= factor; // zoom in by requested factor in both x and y
  scaleLat /= factor;
  setView();
}
void MapViewer::zoomOutOnCenter(float lat, float lon, float factor) {
  zoomInOnCenter(lat, lon, 1/factor); // zoom out by the inverse of zoomIn?
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

void MapViewer::setOriginalCoords(float centerLat, float centerLon, float scaleLat, float scaleLon, float shiftLat, float shiftLon) {
  origCenterLat = centerLat;
  origCenterLon = centerLon;
  origScaleLat = scaleLat;
  origScaleLon = scaleLon;
  origShiftLat = shiftLat;
  origShiftLon = shiftLon;
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

void MapViewer::setTextScale(float textScale) {
  this->textScale = textScale;
  mv->setTextScale(1/textScale);
  setView();
}

void MapViewer::increaseTextSize(float factor) {
  textScale *= factor;
  setTextScale(textScale);
}

void MapViewer::decreaseTextSize(float factor) {
  increaseTextSize(1/factor);
}

void MapViewer::toggleDisplayText() {
  displayText = !displayText;
  setView();
}

void MapViewer::toggleDisplayOutline() {
  displayOutline = !displayOutline;
  setView();
}

void MapViewer::toggleDisplayFill() {
  displayFill = !displayFill;
  setView();
}

//TODO: why does this translate where we are looking?
void MapViewer::zoomInOnMouse(float factor) {
  //glm::mat4 inverseTrans = glm::inverse(trans);
  //glm::vec4 mouseVec = inverseTrans * glm::vec4(w->mouseX, w->mouseY, 0, 1);
  BoundRect r(centerLon - scaleLon + shiftLon, centerLon + scaleLon + shiftLon, centerLat - scaleLat + shiftLat,
                     centerLat + scaleLat + shiftLat);
  float mouseFracX = (float)w->mouseX / (w->width-1);
  float mouseFracY = (float)w->mouseY / (w->height-1);
  float mouseVecX = (r.xMin * (1-mouseFracX)) + (r.xMax * mouseFracX);
  float mouseVecY = (r.yMin * (1-mouseFracY)) + (r.yMax * mouseFracY);
  std::cout << "x= " << mouseVecX << " y= " << mouseVecY << std::endl;
  zoomInOnCenter(mouseVecY, mouseVecX, factor);
}

void MapViewer::zoomOutOnMouse(float factor) {
  zoomInOnMouse(1/factor);
}

void MapViewer::incSegment(){
  mv->incSegment();
  setView();
}

void MapViewer::decSegment() {
  mv->decSegment();
  setView();
}

void MapViewer::incNumSegments() {
  mv->incNumSegments();
  setView();
}

void MapViewer::decNumSegments() {
  mv->decNumSegments();
  setView();
}

void MapViewer::displayAllSegments() {
  mv->displayAllSegments();
  setView();
}

void MapViewer::displayFirstSegment() {
  mv->setWhichSegmentsToDisplay(50,100);
  setView();
}

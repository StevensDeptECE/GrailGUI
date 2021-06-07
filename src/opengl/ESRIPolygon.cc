#include "Shapefile.hh"

ESRIPolygon::ESRIPolygon(SHPObject* shapePtr) {
  ESRIShape::shapePtr = shapePtr;
  ESRIShape::shapeType = shapePtr->nSHPType;
  //std::vector<int> parts;
  //std::vector<ESRIPoint> points;
  numParts = shapePtr->nParts;
  numPoints = shapePtr->nVertices;

  //TODO: Is this really the best way to do this without four assignments?
  double boundingBoxTmp[4] = {shapePtr->dfXMin, shapePtr->dfXMax,
                              shapePtr->dfYMin, shapePtr->dfYMax};
  std::copy(&boundingBoxTmp[0], &boundingBoxTmp[4], boundingBox);
  parts = shapePtr->panPartStart;
  xPoints = shapePtr->padfX;
  yPoints = shapePtr->padfY;
}

ESRIPolygon::~ESRIPolygon() {
  //free(parts);
  //free(xPoints);
  //free(yPoints);
  free(shapePtr);
}

std::vector<std::vector<double>> ESRIPolygon::dumpPoints() {
  if (shapePtr == nullptr || xPoints == nullptr || yPoints == nullptr) {
    std::cerr << "Warning: shape pointer or components are null\n";
    return {};
  }

  std::vector<std::vector<double>> points;
  points.reserve(numPoints);

  for (int i = 0; i < numPoints; i++) {
    points.push_back(std::vector<double>(2));
    points[i][0] = xPoints[i];
    points[i][1] = yPoints[i];
  }
  return points;
}
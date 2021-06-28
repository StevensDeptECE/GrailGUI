#pragma once

#include <memory>
#include <vector>

#include "opengl/Errcode.hh"
#include "shapelib/shapefil.h"
#include "util/Ex.hh"

class Shapefile {
 private:
  SHPHandle shapeHandle;
  int nEntities, nShapeType;
  double minBounds[4], maxBounds[4];
  std::vector<SHPObject*> shapeObjects;

 public:
  Shapefile(const char filename[]);
  ~Shapefile();
  void init();
  SHPObject* getShape(int index);
  std::vector<SHPObject*> getShapeVector();
  double* getMinBounds();
  double* getMaxBounds();
};

class ESRIPoint {
 public:
  double x, y;
  ESRIPoint() : x(0), y(0){};
  ESRIPoint(double x, double y) : x(x), y(y){};
};

class ESRIShape {
 protected:
  SHPObject* shapePtr;

 public:
  int shapeType;
  ESRIShape() : shapePtr(NULL), shapeType(0){};
  ESRIShape(SHPObject* shapePtr)
      : shapePtr(shapePtr), shapeType(shapePtr->nSHPType){};
  static std::vector<std::unique_ptr<ESRIShape>> convertSHPObjects(
      const std::vector<SHPObject*>& shapeObjects);
  virtual std::vector<ESRIPoint> getPoints() = 0;
};

class ESRIPolygon : public ESRIShape {
 private:
  double boundingBox[4];
  int numParts, numPoints;
  int* parts;
  double *xPoints, *yPoints;
  std::vector<ESRIPoint> points;

 public:
  ESRIPolygon(SHPObject* shapePtr)
      : ESRIShape(shapePtr),
        boundingBox{shapePtr->dfXMin, shapePtr->dfXMax, shapePtr->dfYMin,
                    shapePtr->dfYMax},
        numParts(shapePtr->nParts),
        numPoints(shapePtr->nVertices),
        xPoints(shapePtr->padfX),
        yPoints(shapePtr->padfY){};
  std::vector<ESRIPoint> getPoints();
};

// TODO: Implement classes for individual ESRIShapes?

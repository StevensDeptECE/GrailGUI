#pragma once

#include <vector>

#include "libshape/shapefil.h"
#include "opengl/Errcode.hh"
#include "util/Ex.hh"

class Shapefile {
 private:
  SHPHandle shapeHandle;
  int nEntities, nShapeType;
  double minBounds[4], maxBounds[4];
  std::vector<SHPObject*> shapeObjects;

 public:
  Shapefile(const char filename[]);
  void init();
  SHPObject* getShape(int index);
  std::vector<SHPObject*> getShapeVector();
};

class ESRIShape {
 protected:
  SHPObject* shapePtr;

 public:
  int shapeType;
  ESRIShape() : shapePtr(NULL){};
  ESRIShape(SHPObject* shapePtr) : shapePtr(shapePtr), shapeType(shapePtr->nSHPType){};
  static std::vector<ESRIShape*> convertSHPObjects(std::vector<SHPObject*> shapeObjects);
  virtual std::vector<std::vector<double>> dumpPoints() = 0;
};

class ESRIPolygon : public ESRIShape {
 private:
  double boundingBox[4];
  int numParts, numPoints;
  int* parts;
  double *xPoints, *yPoints;

 public:
  ESRIPolygon(SHPObject* shape);
  std::vector<std::vector<double>> dumpPoints();
};

//TODO: Implement classes for individual ESRIShapes?
/*
class ESRIPoint : public ESRIShape {
 private:
  double x, y;
  double coordPair[2];

 public:
  ESRIPoint(double x, double y) : x(x), y(y){};
  std::vector<std::vector<double>> dumpPoints();
};
*/
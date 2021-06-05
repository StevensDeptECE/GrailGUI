#pragma once

#include <vector>

#include "libshape/shapefil.h"
#include "opengl/Errcode.hh"
#include "util/Ex.hh"

//TODO: Add destructors
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
  //double[][] getBounds
};

//TODO: Ensure that pointer built by convertShapePointer is destroyed
/* Valgrind record from shapes.
==78176== 39,301,944 bytes in 3,216 blocks are indirectly lost in loss record 25 of 26
==78176==    at 0x48435FF: calloc (vg_replace_malloc.c:1117)
==78176==    by 0x4A58F28: SHPReadObject (in /usr/local/lib/libshp.so.1.5.0)
==78176==    by 0x10BB8F: Shapefile::init() (Shapefile.cc:23)
==78176==    by 0x10A40B: main (TestESRIPolygon.cc:9)
==78176==
==78176== 157,734,432 (283,360 direct, 157,451,072 indirect) bytes in 3,220 blocks are definitely lost in loss record 26 of 26
==78176==    at 0x483EF3F: operator new(unsigned long) (vg_replace_malloc.c:417)
==78176==    by 0x10D144: ESRIShape::convertSHPObjects(std::vector<tagSHPObject*, std::allocator<tagSHPObject*> >) (ESRIShape.cc:8)
==78176==    by 0x10A43D: main (TestESRIPolygon.cc:10)
*/
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
#include "opengl/Shapefile.hh"

std::vector<ESRIShape*> ESRIShape::convertSHPObjects(std::vector<SHPObject*> shapeObjects) {
  std::vector<ESRIShape*> esriShapes(shapeObjects.size());
  for (int i = 0; i < shapeObjects.size(); i++) {
    switch (shapeObjects[i]->nSHPType) {
      case SHPT_POLYGON:
        esriShapes[i] = new ESRIPolygon(shapeObjects[i]);
        break;
      default:
        throw Ex1(Errcode::UNDEFINED_TYPE);
    }
  }
  return esriShapes;
}
#include "opengl/Shapefile.hh"

std::vector<std::unique_ptr<ESRIShape>> ESRIShape::convertSHPObjects(
    const std::vector<SHPObject*>& shapeObjects) {
  std::vector<std::unique_ptr<ESRIShape>> esriShapes(shapeObjects.size());
  for (int i = 0; i < shapeObjects.size(); i++) {
    switch (shapeObjects[i]->nSHPType) {
      case SHPT_POLYGON:
        esriShapes[i] =
            std::make_unique<ESRIPolygon>(ESRIPolygon(shapeObjects[i]));
        break;
      default:
        throw Ex1(Errcode::UNDEFINED_TYPE);
    }
  }
  return esriShapes;
}

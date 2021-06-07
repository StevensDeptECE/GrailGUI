#include "Shapefile.hh"

std::vector<ESRIPoint> ESRIPolygon::getPoints() {
  // Return if any values are null, throw warning as well
  // This currently does not throw an error
  if (shapePtr == nullptr || xPoints == nullptr || yPoints == nullptr) {
    std::cerr << "Warning: shape pointer or components are null\n";
    return {};
  }

  // Vector size is know ahead of time, so preallocate
  std::vector<ESRIPoint> points(numPoints);

  // Load each pair into vector
  for (int i = 0; i < numPoints; i++) {
    points[i] = ESRIPoint(xPoints[i], yPoints[i]);
  }
  return points;
}

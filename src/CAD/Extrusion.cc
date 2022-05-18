#define _USE_MATH_DEFINES
#include "./Extrusion.hh"

#include <cmath>

#include "./Facet.hh"
#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"

using namespace std;

void Extrusion::extrude() {
  for (uint32_t i = 0; i < size; i += 1) {
    std::vector<Vec3D> newPoints;
    // get the offset between the starting point and next point
    Vec3D offset = points[i] - points[0];

    // TODO: Translate then rotate
    // add the offset to each of the points and store in newPoints
    for (uint32_t j = 0; j < vertices; j += 1) {
      Vec3D temp = fpoints[j] + offset;
      newPoints.push_back(temp);
    }

    // create the facet and add it to the canvas layer
    Facet* newFacet = new Facet(newPoints, c, s);
    c->addLayer(newFacet);
  }
}

void Extrusion::init() {}
void Extrusion::render() {}
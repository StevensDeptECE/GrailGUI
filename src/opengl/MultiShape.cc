#include "opengl/MultiShape.hh"

MultiShape::~MultiShape() {}

void MultiShape::process_input(Inputs* in, float dt) {}

void MultiShape::update() {
  updatePoints();
  updateSolidIndices();
  updateLineIndices();
  updatePointIndices();
}

void MultiShape::updatePoints() {
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0],
               GL_DYNAMIC_DRAW);
}

void MultiShape::updateSolidIndices() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * solidIndices.size(),
               &solidIndices[0], GL_DYNAMIC_DRAW);
}

void MultiShape::updateLineIndices() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * lineIndices.size(),
               &lineIndices[0], GL_DYNAMIC_DRAW);
}

void MultiShape::updatePointIndices() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * pointIndices.size(),
               &pointIndices[0], GL_DYNAMIC_DRAW);
}

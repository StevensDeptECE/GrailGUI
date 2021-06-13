#include "opengl/MapView2D.hh"

#include "opengl/BlockMapLoader.hh"  //TODO: move to util or new data directory

void MapView2D::init() {
  numPoints = bml->getNumPoints();
  glGenVertexArrays(1, &vao);  // Create the container for all vbo objects
  glBindVertexArray(vao);

  // push points up to graphics card as two separate vbo for x and y
  glGenBuffers(1, &vbox);
  glBindBuffer(GL_ARRAY_BUFFER, vbox);
  glBufferData(GL_ARRAY_BUFFER, numPoints * sizeof(float), bml->getXPoints(),
               GL_STATIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

  glGenBuffers(1, &vboy);
  glBindBuffer(GL_ARRAY_BUFFER, vboy);
  glBufferData(GL_ARRAY_BUFFER, numPoints * sizeof(float), bml->getYPoints(),
               GL_STATIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

  // Create a buffer object for indices of lines
  uint32_t numSegments = bml->getNumSegments();
  constexpr uint32_t endIndex = 0xFFFFFFFF;
  const uint32_t numIndices = numPoints + numSegments;
  uint32_t* lineIndices = new int[numIndices];
  for (uint32_t i = 0, j = 0, c = 0; i < numSegments; i++) {
    for (uint32_t k = 0; k < bml->getSegment(i).numPoints; k++)
      lineIndices[c++] = j++;
    lineIndices[c++] = endIndex;
  }
  glGenBuffers(1, &lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndices,
               lineIndices, GL_STATIC_DRAW);

  delete[] lineIndices;
}

void MapView2D::render() {
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFFU);

  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection", *parentCanvas->getProjection() * transform);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glLineWidth(style->getLineWidth());

  // Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_LINE_LOOP, numPoints, GL_UNSIGNED_INT, 0);

  // Unbind
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDisable(GL_PRIMITIVE_RESTART);
}

void MapView2D::dump() {}

#include "ColorGrid.hh"

#include "glad/glad.h"
ColorGrid::~ColorGrid() {}

void ColorGrid::init() {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);  // vbo stores all the vertex values
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0],
               GL_STATIC_DRAW);
  // each point is x,y
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glGenBuffers(1, &indbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),
               &indices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &cbo);
  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(),  // RGB
               &colors[0], GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void ColorGrid::render() {
  shader->setMat4("projection", *(parentCanvas->getProjection()));
  shader->setVec3("solidColor", style->getFgColor());

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);

  glBindBuffer(GL_ARRAY_BUFFER, cbo);
  glColorPointer(3, GL.GL_FLOAT, 0, 0);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  // Unbind
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

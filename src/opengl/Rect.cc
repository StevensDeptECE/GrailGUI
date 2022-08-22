#include "opengl/Rect.hh"

#include "glad/glad.h"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"

void Rect::init() {
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create VBO for vertices
  // Create an object in the VAO to store all the vertex values
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  float xy[20] = {x,         y, r, g, b, x,         y + height, r, g, b,
                  x + width, y, r, g, b, x + width, y + height, r, g, b};
  glBufferData(
      GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), xy,
      GL_DYNAMIC_DRAW);  // TODO: try with GL_STATIC_DRAW. Is it faster?
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(2 * sizeof(float)));
}

void Rect::update() {}

void Rect::render(glm::mat4& trans) {
  Shader* s = Shader::useShader(GLWin::PER_VERTEX_SHADER);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

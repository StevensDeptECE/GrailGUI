#include "opengl/Rectangle.hh"

#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"

Rectangle::Rectangle(Canvas* c, float x, float y, float width, float height, const glm::vec4& color)
      : Shape(c), fillColor(color) {
    float points[8] = {x, y, x+width, y, x+width, y+height, x, y+height};
  glGenVertexArrays(1, &vao);  // Create the container for all vbo objects
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), points, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void Rectangle::init() {
}

void Rectangle::update() {
}

void Rectangle::render(glm::mat4& trans) {
  // Get Shader based on style
  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("trans", trans);
  shader->setVec4("solidColor", fillColor);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
}
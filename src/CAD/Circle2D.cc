#include "CAD/Circle2D.hh"

#include <unistd.h>

#include <cmath>
#include <numbers>

#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"

using namespace std::numbers;
Circle2D::Circle2D(Canvas* c, float x, float y, float radius,
                   uint32_t numOfTriangles, const Style* s)
    : Shape2D(c, x, y, s),
      x(x),
      y(y),
      numOfTriangles(numOfTriangles),
      radius(radius) {}

void Circle2D::init() {
  // Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLfloat circleVertX[362];  // vertecies
  GLfloat circleVertY[362];

  circleVertX[0] = x;
  circleVertY[0] = y;
  // glVertex2f(x,y);
  for (int i = 1; i < 362; i++) {
    circleVertX[i] = x + (radius * cos(i * (2 * pi / numOfTriangles)));
    circleVertY[i] = y + (radius * sin(i * (2 * pi / numOfTriangles)));
  }

  // GLfloat allV[vertecies*2];
  // allVerts = allV[vertecies*2];
  //  circleVerts[742];
  for (int i = 0; i < 362; i++) {
    circleVerts[i * 2] = circleVertX[i];
    circleVerts[(i * 2) + 1] = circleVertY[i];
  }

  // Create VBO for vertices
  // Create an object in the VAO to store all the vertex values
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 724 * sizeof(float), &circleVerts,
               GL_DYNAMIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void Circle2D::render() {
  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection", *(parentCanvas->getProjection()));
  shader->setVec4("solidColor", style->getFgColor());

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_TRIANGLE_FAN, 1, 724);
  // glDrawArrays(GL_LINE_LOOP, 0, 3);
  // glDrawArrays(GL_POINTS, 0, 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

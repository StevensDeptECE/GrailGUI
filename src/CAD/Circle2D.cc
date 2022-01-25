#include "CAD/Circle2D.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include <cmath>
#include <numbers>

using namespace std::numbers;
Circle2D::Circle2D(Canvas* c, float x, float y,float radius, uint32_t numOfTriangles, const Style* s) :
    Shape2D(c, x, y, s) , x(x) , y(y), numOfTriangles(numOfTriangles), radius(radius){}

void Circle2D::init() {
  // Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  //GLint vertecies = segments + 2;
  //const GLfloat pi = 2.0f * 3.1415926f;
  GLfloat circleVertX[numOfverticies];// vertecies
  GLfloat circleVertY[numOfverticies];

  circleVertX[0] = x;
  circleVertY[0] = y;
  glVertex2f(x,y);
  for(int i = 1; i < numOfverticies; i++)
  {
    circleVertX[i] = x +  (radius * cos( i * (2 * pi / numOfTriangles)));
    circleVertY[i] = y +  (radius * sin( i * (2 * pi / numOfTriangles)));

  }

  //GLfloat allV[vertecies*2];
  //allVerts = allV[vertecies*2];
  for(int i = 0; i < numOfverticies; i++) {
    circleVerts[i*2] = circleVertX[i];
    circleVerts[(i*2)+1] = circleVertY[i];
  }

  // Create VBO for vertices
  // Create an object in the VAO to store all the vertex values
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, numOfverticies *2 * sizeof(float), &circleVerts, GL_DYNAMIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
}

void Circle2D::render(){
  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_TRIANGLE_FAN, 0, numOfverticies);
  //glDrawArrays(GL_LINE_LOOP, 0, 3);
  //glDrawArrays(GL_POINTS, 0, 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

// void Circle2D::drawCircle()
// {
//   Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
//   shader->setMat4("projection",*(parentCanvas->getProjection()));
// 	shader->setVec4("solidColor",style->getFgColor());

//   glBindVertexArray(vao);
//   glEnableVertexAttribArray(0);
//   glLineWidth(style->getLineWidth());

//     glBegin(GL_LINE_LOOP);
//     for (int ii = 0; ii < segments; ii++)   {
//         float theta = 2.0f * 3.1415926f * float(ii) / float(segments);//get the current angle 
//         float x2 = radius * cosf(theta);//calculate the x component 
//         float y2 = radius * sinf(theta);//calculate the y component 
//         glVertex2f(x2 + x, y2 + y);//output vertex 
//     }
//     glEnd();
//   glDisableVertexAttribArray(0);
//   glBindVertexArray(0);
// }
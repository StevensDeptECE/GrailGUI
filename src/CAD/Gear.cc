#include "CAD/Gear.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include <cmath>
#include <numbers>

Gear::Gear(Canvas* c, float x, float y,float innerRadius, float outterRadius, float numTeeth, float toothDepth, float width, const Style* s) :
    Shape2D(c, x, y, s) , x(x) , y(y), innerRadius(innerRadius), outterRadius(outterRadius), numTeeth(numTeeth), toothDepth(toothDepth), width(width){}

void Gear::init(){
  // Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  float radius0 = innerRadius;
  float radius1 = outterRadius - toothDepth / 2.0;
  float radius2 = outterRadius + toothDepth / 2.0;

  float da = 2.0 / M_PI / numTeeth / 4.0;
  std::vector<float> verticies;

  for(int i = 0; i <= numTeeth; i++) {
    float angle = i * 2.0 * M_PI / numTeeth;
    verticies.push_back(x + radius0 * cos(angle));
    verticies.push_back(y + radius0 * sin(angle));
    verticies.push_back(x+radius1 * cos(angle));
    verticies.push_back(y+radius0 * sin(angle));
    //glVertex3f(radius0 * cos(angle), radius0 * sin(angle), width * 0.5);
    //glVertex3f(radius1 * cos(angle), radius1 * sin(angle), width * 0.5);
    if (i < numTeeth) {
      verticies.push_back(x + radius0 * cos(angle));
      verticies.push_back(y + radius0 * sin(angle))
      verticies.push_back(x + radius1 * cos(angle + 3 * da));
      verticies.push_back(y +radius1 * sin(angle + 3 * da));

     // glVertex3f(radius0 * cos(angle), radius0 * sin(angle), width * 0.5);
      //glVertex3f(radius1 * cos(angle + 3 * da), radius1 * sin(angle + 3 * da), width * 0.5);
    }
  }

  // Create VBO for vertices
  // Create an object in the VAO to store all the vertex values
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &x1, GL_DYNAMIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

}

void Gear::render(){
  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  glDrawArrays(GL_TRIANGLES, 0, 3);
  //glDrawArrays(GL_LINE_LOOP, 0, 3);
  //glDrawArrays(GL_POINTS, 0, 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}
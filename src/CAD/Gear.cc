#include <unistd.h>
#include "CAD/Gear.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLWin.hh"
#include "opengl/Canvas.hh"
#include <cmath>
#include <numbers>

using namespace std::numbers;


Gear::Gear(Canvas* c, float x, float y,float innerRadius, float outterRadius, float numTeeth, float toothDepth, float width, const Style* s) :
    Shape2D(c, x, y, s) , x(x) , y(y), innerRadius(innerRadius), outterRadius(outterRadius), numTeeth(numTeeth), toothDepth(toothDepth), width(width){}

void Gear::init(){
  // Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  float radius0 = innerRadius;
  float radius1 = outterRadius;
  float radius2 = outterRadius + toothDepth;

  float da = 2.0 / pi / numTeeth / 4.0;

  float first_spot = 0;
  float sec_spot = 0;

  for(int i = 0; i < numTeeth; i++) {
    float angle = i * 2.0 * pi / numTeeth;
    first_spot = x + radius0 * cos(angle);
    verticies.push_back(x + radius0 * cos(angle));
    sec_spot = y + radius0 * sin(angle);
    verticies.push_back(y + radius0 * sin(angle));
    verticies.push_back(x + radius1 * cos(angle));
    verticies.push_back(y + radius0 * sin(angle));
    if (i < numTeeth) {
      verticies.push_back(x + (radius0 * cos(angle)));
      verticies.push_back(y + (radius0 * sin(angle)));
      verticies.push_back(x + (radius1 * cos(angle + 3 * da)));
      verticies.push_back(y + (radius1 * sin(angle + 3 * da)));
   }
  }
  verticies.push_back(first_spot);
  verticies.push_back(sec_spot);


  da = 2.0 *  pi / numTeeth / 4.0;
  for (int i = 0; i < numTeeth; i++) {
    float angle = i * 2.0 * pi / numTeeth;
    verticies.push_back(x + radius1 * cos(angle));
    verticies.push_back(y + radius1 * sin(angle));
    verticies.push_back(x + radius2 * cos(angle + da));
    verticies.push_back(y + radius2 * sin(angle + da));
    verticies.push_back(x + radius2 * cos(angle + 2 * da));
    verticies.push_back(y + radius2 * sin(angle + 2 * da));
    verticies.push_back(x + radius1 * cos(angle + 3 * da));
    verticies.push_back(y + radius1 * sin(angle + 3 * da));
    
  }
  
  std::cout << verticies.size() << std::endl;

  // for(int i =0; i <verticies.size(); i++)
  // {
  //   std::cout << verticies[i] << std::endl;
  // }
  // std::cout << verticies[-1] << std::endl;
  // std::cout << verticies[-2] << std::endl;
  // std::cout << verticies[-3] << std::endl;
  // std::cout << verticies[-4] << std::endl;
  // std::cout << verticies[-5] << std::endl;
  // verticies.push_back(verticies[-2]);
  // verticies.push_back(verticies[-1]);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_DYNAMIC_DRAW);
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
  glDrawArrays(GL_LINE_LOOP, 0, verticies.size());//verticies.size());
  //glDrawArrays(GL_LINE_LOOP, 0, 3);
  //glDrawArrays(GL_POINTS, 0, 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}
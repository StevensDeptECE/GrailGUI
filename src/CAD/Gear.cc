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

  for(int i = 0; i < numTeeth; i++) {
    float angle = i * 2.0 * pi / numTeeth;
    std::cout << x + radius0 * cos(angle) << std::endl;
    verticies.push_back(x + radius0 * cos(angle));
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

  da = 2.0 *  pi / numTeeth / 4.0;
  for (int i = 0; i < numTeeth; i++) {
    float angle = i * 2.0 * pi / numTeeth;
    std::cout << "new" << std::endl;
    verticies.push_back(x + radius1 * cos(angle));
    std::cout << x + radius1 * cos(angle) << std::endl;
    verticies.push_back(y + radius1 * sin(angle));
    std::cout << y + radius1 * sin(angle) << std::endl;
    verticies.push_back(x + radius2 * cos(angle + da));
    std::cout << x + radius2 * cos(angle + da) << std::endl;
    verticies.push_back(y + radius2 * sin(angle + da));
    std::cout << y + radius2 * sin(angle + da) << std::endl;
    verticies.push_back(x + radius2 * cos(angle + 2 * da));
    std::cout << x + radius2 * cos(angle + 2 * da) << std::endl;
    verticies.push_back(y + radius2 * sin(angle + 2 * da));
    std::cout << y + radius2 * sin(angle + 2 * da) << std::endl;
    verticies.push_back(x + radius1 * cos(angle + 3 * da));
    std::cout << x + radius1 * cos(angle + 3 * da) << std::endl;
    verticies.push_back(y + radius1 * sin(angle + 3 * da));
    std::cout << y + radius1 * sin(angle + 3 * da) << std::endl;
    
  }
  


  // GLfloat circleVertX[362];// vertecies
  // GLfloat circleVertY[362];

  // circleVertX[0] = x;
  // circleVertY[0] = y;
  // //glVertex2f(x,y);
  // for(int i = 1; i < 362; i++)
  // {
  //   circleVertX[i] = x +  (outterRadius * cos( i * (2 * 3.14159265358979323846 / 362)));
  //   circleVertY[i] = y +  (outterRadius * sin( i * (2 * 3.14159265358979323846 / 362)));

  // }

  // for(int i = 0; i < 362; i++) {
  //   verticies.push_back(circleVertX[i]);
  //   verticies.push_back(circleVertY[i]);
  // }
  for(int i =0; i <verticies.size(); i++)
  {
    std::cout << verticies[i] << std::endl;
  }
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
  glDrawArrays(GL_LINE_LOOP, 1, verticies.size()-123);//verticies.size());
  //glDrawArrays(GL_LINE_LOOP, 0, 3);
  //glDrawArrays(GL_POINTS, 0, 3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}
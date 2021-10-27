#include "CAD/Triangle.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"


void Triangle::render(){
  Shader * s = Shader::useShader(style->getShaderIndex());

  applyTransform(s);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  //If color buffer exists
  if(cbo>0){
    glEnableVertexAttribArray(1);
  }else{
    s->setVec4("solidColor",style->getFgColor());
  }
  glLineWidth(style->getLineWidth());

  //Draw Solids
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sbo);
  glDrawElements(GL_TRIANGLES,solidIndices.size(),GL_UNSIGNED_INT,0);

  //Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glDrawElements(GL_LINES,lineIndices.size(),GL_UNSIGNED_INT,0);

  //Draw Points
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pbo);
  glDrawElements(GL_POINTS,pointIndices.size(),GL_UNSIGNED_INT,0);

  //Unbind
  if(cbo>0)
    glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

  void Triangle::initIndices(){
  for(int i = 0;i<vertices.size();i++){
    lineIndices.push_back(i);
    pointIndices.push_back(i);
  }

  solidIndices.push_back(0);
  solidIndices.push_back(1);
  solidIndices.push_back(2);
  solidIndices.push_back(0);
}

float getArea(float base, float height){
    return (.5 * base * height);
}

float getArea(float x, float y,float x2, float y2,float x3, float y3){
  //write formula
}
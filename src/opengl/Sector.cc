#include "opengl/Sector.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"

void Sector::initIndices(){
  solidIndices.push_back(0);

  for(uint32_t i = 1; i<vertices.size()/2;i++){
    solidIndices.push_back(i);
    lineIndices.push_back(i);
    pointIndices.push_back(i);
  }
}

void Sector::render(){

  //Get Shader based on style
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

  ////Draw Solids
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sbo);
 glDrawElements(GL_TRIANGLE_FAN,solidIndices.size(),GL_UNSIGNED_INT,0);

 //Draw Lines
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
 glDrawElements(GL_LINE_STRIP,lineIndices.size(),GL_UNSIGNED_INT,0);

  //Draw Points
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pbo);
  glDrawElements(GL_POINTS,pointIndices.size(),GL_UNSIGNED_INT,0);

  //Unbind
  if(cbo>0)
    glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

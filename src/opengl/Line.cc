#include "Line.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"

void Line::render(){
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

void Line::initIndices(){
  for(uint32_t i = 0; i<vertices.size();i++){
    lineIndices.push_back(i);
    pointIndices.push_back(i);
  }
}

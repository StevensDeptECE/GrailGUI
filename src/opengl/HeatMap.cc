#include "glad/glad.h"
#include "opengl/GLMath.hh"
#include "opengl/Shader.hh"
#include "opengl/Style.hh"
#include "opengl/StyledMultiShape2D.hh"
//#include "grail/Grail.hh"
#include "opengl/HeatMap.hh"
#include "opengl/Shape_impl.hh"
HeatMap::~HeatMap() {}

void HeatMap::render(glm::mat4& trans) {
  // Get Shader based on style
  Shader* shader = Shader::useShader(GLWin::HEATMAP_SHADER);
  shader->setMat4("projection", trans);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glEnableVertexAttribArray(1);

  //  glLineWidth(style->getLineWidth());

  /*
    0    1    2    3    4
    5    6    7    8    9
    10  11   12   13    14

    draw recet 0,1,5,6  1,2,6,7, ....


  */
  // Draw Solids
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glDrawElements(GL_TRIANGLES, grid.size(), GL_UNSIGNED_INT, 0);
#if 0
  //Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glDrawElements(GL_LINES,lineIndices.size(),GL_UNSIGNED_INT,0);
#endif

  // Unbind
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

void HeatMap::init() {
  // Create VAO,
  //  a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create VBO for vertices
  // Create an object in the VAO to store all the vertex values
  gen(vbo, grid, 3*sizeof(float),    0, 2, 0,     1, 1, 2*sizeof(float));
  // Desctribe how information is recieved in shaders
  // no indices for now.  Not the most efficient way to do this.... gen(sbo,
  // indices);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

#if 0
  //Create LBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as lines(wireframe)
  glGenBuffers(1,&lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*lineIndices.size(),&lineIndices[0],GL_DYNAMIC_DRAW);
#endif
#if 0
// TODO: do we need to unbind at the end of init?
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
#endif
}

void HeatMap::update() {}

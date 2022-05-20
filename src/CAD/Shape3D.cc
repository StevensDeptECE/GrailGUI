// #include "CAD/Shape3D.hh"
// #include "opengl/Shader.hh"

// #include "glad/glad.h"

// void Shape3D::applyTransform(Shader* s){
//   s->setMat4("trans", transform->getTransform());
// }

// void Shape3D::~Shape3D(){

// }
// void Shape2D::init(){
//   //Create VAO,
//   // a container to have all shapes and their attributes
//   glGenVertexArrays(1,&vao);
//   glBindVertexArray(vao);

//   //Create VBO for vertices
//   //Create an object in the VAO to store all the vertex values
//   glGenBuffers(1,&vbo);
//   glBindBuffer(GL_ARRAY_BUFFER,vbo);
//   glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vertices.size(),&vertices[0],GL_STATIC_DRAW);
//   //Desctribe how information is recieved in shaders
//   glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

//   //Create SBO
//   //Create an object to hold the order at which the vertices are drawn(from
//   indices)
//   //in order to draw it as a solid(filled)
//   glGenBuffers(1,&sbo);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sbo);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//   sizeof(GLuint)*solidIndices.size(),&solidIndices[0],GL_STATIC_DRAW);

//   //Create LBO
//   //Create an object to hold the order at which the vertices are drawn(from
//   indices)
//   //in order to draw it as lines(wireframe)
//   glGenBuffers(1,&lbo);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//   sizeof(GLuint)*lineIndices.size(),&lineIndices[0],GL_STATIC_DRAW);

//   //Create PBO
//   //Create an object to hold the order at which the vertices are drawn(from
//   indices)
//   //in order to draw it as points.
//   glGenBuffers(1,&pbo);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pbo);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER,
//   sizeof(GLuint)*pointIndices.size(),&pointIndices[0],GL_STATIC_DRAW);

//   //Create CBO if exists
//   //Create an object that will hold the rgb information per vertex if
//   //the colors vector hold more than 3 floats(rgb)
//   if(colors.size()>2){
//     glGenBuffers(1,&cbo);
//     glBindBuffer(GL_ARRAY_BUFFER,cbo);
//     glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*colors.size(),&colors[0],GL_STATIC_DRAW);
//     //Desctribe how information is recieved in shaders
//     glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

//   }

//   //Unbind the buffers and vertex array
//   glBindBuffer(GL_ARRAY_BUFFER,0);
//   glBindVertexArray(0);
// }

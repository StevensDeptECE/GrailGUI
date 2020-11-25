#include "opengl/Sphere.hh"
#include "opengl/GLMath.hh"
//const double PI = 3.14159265358979;

Sphere::Sphere(uint32_t latRes, uint32_t lonRes) : latRes(latRes), lonRes(lonRes) {
}
Sphere::~Sphere() {
}

inline void Sphere::addVert(float x, float y, float z) {
  vert.push_back(x);
  vert.push_back(y);
  vert.push_back(z);
}

inline void Sphere::addRect(uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
  ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
  ind.push_back(i1);  ind.push_back(i3);  ind.push_back(i4);
}

inline void Sphere::addTri(uint32_t i1, uint32_t i2, uint32_t i3) {
  ind.push_back(i1);  ind.push_back(i2);  ind.push_back(i3);
}

/*
  create a unit sphere centered on the origin in the requested resolution
  for different sized spheres, use transforms.
  All spheres of the same resolution can use a single copy of the coords
*/
void Sphere::init() {
  const double DPHI = PI<double> / latRes; // vertically only +90 to -90 degrees
  const double PHI = -PI<double> / 2 + DPHI;
  const double DTHETA = 2 * PI<double> / lonRes;

  //preallocate enough space for all the points for a wireframe sphere
  vert.reserve((latRes * lonRes + 2) * 3); // *3 because each point is x,y,z

  //preallocate enough space for the central band of rectangles, each 2 triangles *6
  // plus the endcaps, each latRes triangles *3 (x,y,z) * 2 (north and south poles)
  ind.reserve(latRes * lonRes * 6 + 2*latRes*3);
  // first define all the points for the sphere

  // first step all the points going around a band except North and South poles
  for (int j = 0; j < latRes - 1; ++j, phi += dphi) {
    const double z = sin(phi);
    const double r2 = cos(phi);
    double theta = 0;
    for (int i = 0; i < lonRes; ++i, theta += DTHETA) {
      double x = r2 * cos(theta), y = r2 * sin(theta);
      addVert(x,y,z); // first compute all points
    }
  }

  // finally add the two special cases, south and north poles
  addVert(0,0,-1); // south pole
  addVert(0,0,+1); // north pole

  //    Next, create the indices for the triangles for each grid in the central band
  for (int j = 0, row = 0; j < latRes - 1; ++j, row += lonRes) {
    for (int i = lonRes-1; i > 0; --i)
      addRect(row+i-1, row+i, row+i+lonRes, row+i-1+lonRes);
    addRect(row+lonRes-1, row, row+lonRes, row + lonRes + lonRes - 1);
  }

  const uint32_t S = latRes * lonRes, N = S + 1;

  // add the indices for the south cap
   for (int i = 1; i < lonRes; i++)
    addTri(S, i-1, i);
  addTri(S, lonRes-1, 0);

  // add the indices for the north cap
  const uint32_t NORTHEDGE = (latRes-1) * lonRes;
  for (int i = 1; i < lonRes; i++)
    addTri(N, NORTHEDGE+i-1, NORTHEDGE+i);
  addTri(N, NORTHEDGE+lonRes-1, NORTHEDGE);

  //Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  //Create VBO for vertices
  //Create an object in the VAO to store all the vertex values
  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vert.size(),&vert[0],GL_STATIC_DRAW);
  //Desctribe how information is received in shaders
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

#if 0
  //Create SBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as a solid(filled)
  glGenBuffers(1,&sbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*solidIndices.size(),&solidIndices[0],GL_STATIC_DRAW);
#endif
  //Create LBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as lines(wireframe)
  glGenBuffers(1,&lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*lineIndices.size(),&lineIndices[0],GL_STATIC_DRAW);

#if 0
  //Create PBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as points.
  glGenBuffers(1,&pbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*pointIndices.size(),&pointIndices[0],GL_STATIC_DRAW);
#endif
}

void Sphere::render() {
  const Shader* shader = Shader::useShader(style->getShaderIndex());
	shader->setVec4("solidColor",style->getFgColor());
	
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glLineWidth(style->getLineWidth());

  //Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glDrawElements(GL_LINES,ind.size(),GL_UNSIGNED_INT,0);

  //Unbind
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

}

void Sphere::cleanup() {

}


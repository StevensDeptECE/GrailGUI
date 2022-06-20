#include "opengl/MapView2D.hh"

#include <iomanip>
#include <iostream>

#include "opengl/Style.hh"
using namespace std;

void MapView2D::init() {
  uint32_t numPoints = bml->getNumPoints();

  glGenVertexArrays(1, &vao);  // Create the container for all vbo objects
  glBindVertexArray(vao);

  // push points up to graphics card as two separate vbo for x and y
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, numPoints * (2 * sizeof(float)),
               bml->getXPoints(), GL_STATIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Create a buffer object for indices of lines
  uint32_t numSegments = bml->getNumSegments();
  constexpr uint32_t endIndex = 0xFFFFFFFF;
  // xy1 xy2 xy3 xy4 xy5 ... (xy points on the map)
  // 1 2 3 4 5 ... 1 0xFFFFFFFF 6 7 8 ... 6 0xFFFFFFFF (connect the points)
  // TODO: not sure why we need numSegments * 3 (crashes with *2 or *1)
  numIndicesToDraw = numPoints + numSegments * 2;
  uint32_t* lineIndices = new uint32_t[numIndicesToDraw];
  uint32_t c = 0;
  for (uint32_t i = 0, j = 0; i < numSegments; i++) {
    uint32_t startSegment = j;
    for (uint32_t k = 0; k < bml->getSegment(i).numPoints; k++)
      lineIndices[c++] = j++;
//    lineIndices[c++] = startSegment; // copy the first index to the end (closes the shape)
    lineIndices[c++] = endIndex; // add the seperator (0xFFFFFFFF)
  }
  glGenBuffers(1, &lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numIndicesToDraw,
               lineIndices, GL_STATIC_DRAW);

  delete[] lineIndices;

  const Font* f = mt->getStyle()->f;
  #if 0
  const BlockMapLoader::Region* regions = bml->getRegions();
  // TODO: need to implement to get states
  //const BlockMapLoader::RegionContainer* regionContainer = bml->getRegionContainer();
  for (uint32_t i = 1; i < bdl->getNodeCount(); i++) {
    const char* name = bdl->getNameAt(i);
    uint32_t len = strlen(name);
    const MapEntry* mapInfo = bdl->getValueAt(i);
    if (mapInfo->entityType == ENT_COUNTY) {
      const BlockMapLoader::Region& r = regions[mapInfo->offset];
      float x = (r.bounds.xMax + r.bounds.xMin)/2;
      float y = (r.bounds.yMax + r.bounds.yMin)/2;
      mt->addCentered(x, y, f, name, len);
    }
  }
  #endif
  mt->addCentered(0,0,f,"testing", 7);
}

void debug(const glm::mat4& m, float x, float y, float z) {
  cout << left << setw(10) << setprecision(2) << m[0][0] << '\t' << m[0][1]
       << '\t' << m[0][2] << '\t' << m[0][3] << '\n'
       << m[1][0] << '\t' << m[1][1] << '\t' << m[1][2] << '\t' << m[1][3]
       << '\n'
       << m[2][0] << '\t' << m[2][1] << '\t' << m[2][2] << '\t' << m[2][3]
       << '\n'
       << m[3][0] << '\t' << m[3][1] << '\t' << m[3][2] << '\t' << m[3][3]
       << '\n';
  glm::vec4 v(x, y, z, 0);
  v = m * v;
  cout << v.x << "," << v.y << "," << v.z << '\n';
}
void MapView2D::render() {
  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setVec4("solidColor", style->getFgColor());

  // shader->setMat4("projection", *parentCanvas->getProjection() * transform);
  // glm::mat4 t = *parentCanvas->getProjection() * transform;
  // debug(transform, 0, 0, 0);
  // debug(t, 100, 0, 0);
  // debug(t, 0, 70, 0);
  shader->setMat4("projection", transform);

#if 0
  // quick debugging rectangle in old immediate mode
  glBegin(GL_QUADS);
  glVertexAttrib2f(0, 0, 0);
  glVertexAttrib2f(0, 200, 0);
  glVertexAttrib2f(0, 200, 200);
  glVertexAttrib2f(0, 0, 200);
  glEnd();
#endif

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFFU);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glLineWidth(style->getLineWidth());

  // Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_LINE_LOOP, numIndicesToDraw, GL_UNSIGNED_INT, 0);

  // Unbind
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDisable(GL_PRIMITIVE_RESTART);
}

void MapView2D::update() {}
void MapView2D::dump() {}

#include "maps/MapView2D.hh"

#include <iomanip>
#include <iostream>

#include "opengl/Style.hh"
#include "maps/MapViewer.hh"
#include "data/BlockMapLoader.hh"
#include "util/BLHashMap.hh"

using namespace std;

MapView2D::MapView2D(MapViewer* parent, const Style* s, MultiText* mt,
 BlockMapLoader* bml, BLHashMap<MapEntry>* bdl, float textScale)
      : Shape(parent), style(s), mt(mt), bml(bml), bdl(bdl),textScale(textScale){
    const BoundRect& bounds = bml->getBlockMapHeader()->bounds;
    parent->setOrigBounds(bounds.xMin, bounds.xMax, bounds.yMin, bounds.yMax);
  }


MapView2D::~MapView2D() {
  delete bml;
  delete bdl;
}

void MapView2D::init() {
  startSegment = 0;
  endSegment = bml->getNumSegments()-1;
  initOutline();
  initLabels(); 
  initFill();
}

void MapView2D::initOutline() {
  uint32_t numPoints = bml->getNumPoints();

  glGenVertexArrays(1, &vao);  // Create the container for all vbo objects
  glBindVertexArray(vao);

  // push points up to graphics card as two separate vbo for x and y
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, numPoints * (2 * sizeof(float)),
               bml->getPoints(), GL_STATIC_DRAW);
  // Describe how information is received in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Create a buffer object for indices of lines
  uint32_t numSegments = bml->getNumSegments();
  constexpr uint32_t endIndex = 0xFFFFFFFF;
  // xy1 xy2 xy3 xy4 xy5 ... (xy points on the map)
  // 1 2 3 4 5 ... 1 0xFFFFFFFF 6 7 8 ... 6 0xFFFFFFFF (connect the points)

  // number of points in blockmap + 1 index per segment for separator - 1 point for the centroid stored after each polygon
  numLineIndicesToDraw = numPoints + numSegments; // so we can also draw centroids if we want to + (numSegments- numSegments);
  uint32_t* lineIndices = new uint32_t[numLineIndicesToDraw];
  uint32_t c = 0;
  for (uint32_t i = 0, j = 0; i < numSegments; i++) {
    for (uint32_t k = 0; k < bml->getSegment(i).numPoints; k++) {
      lineIndices[c++] = j++;
    }
    j++; // skip centroid at end of segment
    lineIndices[c++] = endIndex; // add the separator (0xFFFFFFFF)
  }
  startLineIndex = 0;
  endLineIndex = c;
  numLineIndicesToDraw = endLineIndex - startLineIndex;
  glGenBuffers(1, &lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numLineIndicesToDraw,
               lineIndices, GL_STATIC_DRAW);

  delete[] lineIndices;
}

void MapView2D::initLabels() {
  const Font* f = mt->getStyle()->f;
  const BlockMapLoader::Region* regions = bml->getRegions();
  // TODO: need to implement to get states
  //const BlockMapLoader::RegionContainer* regionContainer = bml->getRegionContainer();
  const uint32_t count = bdl->getNodeCount();
  const float* points = bml->getPoints();
  for (uint32_t i = 1; i < count; i++) {
    const char* name = bdl->getNameAt(i);
    string tempName = name + 2;
    tempName += ":" + to_string(i);
    uint32_t len = strlen(name);
    const MapEntry* mapInfo = bdl->getValueAt(i);
    if (mapInfo->entityType == ENT_COUNTY) {
      const BlockMapLoader::Region& r = regions[mapInfo->offset];
      // scale x and y by 1/factor of projection downscale
      #if 1
      const float* centroidLoc = bml->getSegmentCentroid(r.segmentStart);
      float x = centroidLoc[0]*textScale;
      float y = centroidLoc[1]*-textScale;
      #endif
      //float x = (r.bounds.xMax + r.bounds.xMin)/2*textScale;
      //float y = (r.bounds.yMax + r.bounds.yMin)/2*-textScale;
      mt->addCentered(x, y, f, tempName); // +2 and -2 to remove the appended state abbr.
    }
  }
//  mt->addCentered(0,0,f,"testing", 7);
}

void MapView2D::initFill() {
  uint32_t numPoints = bml->getNumPoints();

  glGenVertexArrays(1, &vaoFill);  // Create the container for all vbo objects
  glBindVertexArray(vaoFill);

  // push points up to graphics card as two separate vbo for x and y
  //glBindBuffer(GL_ARRAY_BUFFER, vbo); // re-use vbo for this solid drawing
  //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  //glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(2*sizeof(float)));

  glGenBuffers(1, &vboFill);
  glBindBuffer(GL_ARRAY_BUFFER, vboFill);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(2*sizeof(float)));

  const float* points = bml->getPoints();
#if 0
  float xyv[] = {
    -150,0, 0,
   -150,55, 0.5,
   -63,0, 0,
   -63,47,0.96};
  glBufferData(GL_ARRAY_BUFFER, 4 * (3 * sizeof(float)),
               xyv, GL_STATIC_DRAW);
#endif
#if 1
  float* xyv = new float[numPoints*3];
  const uint32_t numSegments = bml->getNumSegments();
  int c = 0, d = 0;
  float val = 0;
  for (uint32_t i = 0, j = 0; i < numSegments; i++) {
    for (uint32_t k = 0; k < bml->getSegment(i).numPoints; k++) {
      xyv[c++] = points[d++];  // copy x
      xyv[c++] = points[d++];  // copy y
      xyv[c++] = val; // insert a value
    }
    xyv[c++] = points[d++];  // copy centroid x
    xyv[c++] = points[d++];  // copy centroid y
    xyv[c++] = val; // insert a value (same as for the perimeter)
    val += 0.25;
    if (val > 1)
      val = 0;
  }
  glBufferData(GL_ARRAY_BUFFER, numPoints * (3 * sizeof(float)),
               xyv, GL_STATIC_DRAW);
  cout << "\n\nxyv\n";
  for (int i = 0, j = 0; i < 19; i++, j += 3) {
    cout << xyv[j] << "," << xyv[j+1] << "," << xyv[j+2] << '\n';
  }
  delete[] xyv;
#endif
  // Describe how information is received in shaders
//  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // Create a buffer object for indices of lines
  constexpr uint32_t endIndex = 0xFFFFFFFF;
  // xy1 xy2 xy3 xy4 xy5 ... (xy points on the map)
  // 1 2 3 4 5 ... 1 0xFFFFFFFF 6 7 8 ... 6 0xFFFFFFFF (connect the points)
  // TODO: not sure why we need numSegments * 3 (crashes with *2 or *1)

  //TODO: hopefully right this time? 
  // number of points in blockmap + 1 index per segment for separator
  {
    numFillIndicesToDraw = numPoints + 2*numSegments;// - numSegments);
    uint32_t* fillIndices = new uint32_t[numFillIndicesToDraw];
    uint32_t c = 0;
    for (uint32_t i = 0, j = 0; i < numSegments; i++) {
      if (i == startSegment)
        startFillIndex = c;
      uint32_t start = j; // remember start so it can be copied at the end of the fan
      fillIndices[c++] = j+bml->getSegment(i).numPoints; // Add centroid;
      for (uint32_t k = 0; k < bml->getSegment(i).numPoints; k++) {
        fillIndices[c++] = j++;
      }
      fillIndices[c++] = start;
      j++; //TODO: Daniil sez: put the centroid first to simplify this logic!
      fillIndices[c++] = endIndex; // add the separator (0xFFFFFFFF)
      if (i == endSegment) {
        endFillIndex = c-1; // c is 1 beyond the end of the stored indices
      }
    }
  
  cout << "\nFill indices:\n";
  for (int i = 0; i < 19; i++) {
    cout << fillIndices[i] << ' ';
  }
  cout << "\n";
  glGenBuffers(1, &sbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * numFillIndicesToDraw,
               fillIndices, GL_STATIC_DRAW);

  delete[] fillIndices;
  }

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

void MapView2D::render(glm::mat4& trans) {
  renderOutline(trans);
  renderFill(trans);
}

void MapView2D::renderOutline(glm::mat4& trans) {
  Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setVec4("solidColor", style->getFgColor());

  // shader->setMat4("projection", *parentCanvas->getProjection() * transform);
  // glm::mat4 t = *parentCanvas->getProjection() * transform;
  // debug(transform, 0, 0, 0);
  // debug(t, 100, 0, 0);
  // debug(t, 0, 70, 0);
  shader->setMat4("projection", trans);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFFU);

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glLineWidth(style->getLineWidth());

  // Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_LINE_LOOP, endLineIndex - startLineIndex, GL_UNSIGNED_INT, (void*)(uint64_t)startLineIndex);

  // Unbind
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDisable(GL_PRIMITIVE_RESTART);
}

void MapView2D::renderFill(glm::mat4& trans) {
  
  Shader* shader = Shader::useShader(GLWin::HEATMAP_SHADER);
  shader->setVec3("minColor", glm::vec3(0,0,1));
  shader->setVec3("maxColor", glm::vec3(1,0,0));
  shader->setFloat("minVal", 0.0f);
  shader->setFloat("maxVal", 1.0f);
  shader->setMat4("trans", trans);

  //Shader* shader = Shader::useShader(GLWin::COMMON_SHADER);//TODO: make new shader
  
  //shader->setVec4("solidColor", grail::red); // draw in red, just to see it work!
  //shader->setMat4("projection", trans);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFFU);

  glBindVertexArray(vaoFill);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  // Draw Solid
  //numFillIndicesToDraw = 19;
  //endFillIndex = 19;
  glDrawElements(GL_TRIANGLE_FAN, endFillIndex - startFillIndex, GL_UNSIGNED_INT, (void*)(uint64_t)startFillIndex);
  //glDrawArrays(GL_TRIANGLE_FAN, 0, 4); // quick test, just draw one huge yellow rectangle!
  // Unbind
  //glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
  glDisable(GL_PRIMITIVE_RESTART);
}

void MapView2D::setTextScale(float textScale) {
  this->textScale = textScale;
  mt->clear();
  initLabels();
}

void MapView2D::update() {}
void MapView2D::dump() {}

void MapView2D::setWhichSegmentsToDisplay(uint32_t start, uint32_t end) {
  uint32_t numSegments = bml->getNumSegments();

  startSegment = start;
  if (startSegment == 0xffffffff)
    startSegment = 0;
  if (startSegment > endSegment)
    startSegment = endSegment - 1;
    
  endSegment = end;
  if (endSegment < startSegment)
    endSegment = startSegment + 1;
  if (endSegment > numSegments) {
    endSegment = numSegments;
  }
  uint32_t lineIndex = 0;
  uint32_t fillIndex = 0;
  const BlockMapLoader::Segment* s = bml->getSegments();

  for (uint32_t i = 0; i < startSegment; i++) {
    lineIndex += s[i].numPoints + 1;
    fillIndex += s[i].numPoints + 2; // also add one index to draw the centroid
  }
  startLineIndex = lineIndex; // first position to draw
  startFillIndex = fillIndex;

  for (uint32_t i = startSegment; i < endSegment; i++) {
    lineIndex += s[i].numPoints + 1;
    fillIndex += s[i].numPoints + 2; // also add one index to draw the centroid
  }
  endLineIndex = lineIndex;
  endFillIndex = fillIndex;
  cout << "displaying segments: " << startSegment << " to " << endSegment << 
  " lineIndices=[" << startLineIndex << " " << endLineIndex << 
  "] fillIndices=[" << startFillIndex << " " << endFillIndex << "]" << endl;

}

void MapView2D::displayAllSegments() {
  setWhichSegmentsToDisplay(0, bml->getNumSegments());
}

void MapView2D::incSegment() {
  setWhichSegmentsToDisplay(startSegment+1, endSegment+1);
}

void MapView2D::decSegment() {
  setWhichSegmentsToDisplay(startSegment-1, endSegment-1);
}

void MapView2D::incNumSegments() {
  setWhichSegmentsToDisplay(startSegment, endSegment+1);
}

void MapView2D::decNumSegments() {
  setWhichSegmentsToDisplay(startSegment, endSegment-1);  
}

#include "opengl/StyledMultiShape2D.hh"

#include <vector>

#include "glad/glad.h"
#include "opengl/GLMath.hh"
#include "opengl/Shader.hh"
#include "opengl/Style.hh"

uint32_t StyledMultiShape2D::addSector(float x, float y, float xRad, float yRad, float fromAngle, float toAngle, float angleInc, const glm::vec4& c) {
  for (float i = fromAngle; i <= toAngle; i += angleInc) {
    float px = x + xRad * cos(-i * DEG2RAD<float>);
    float py = y + yRad * sin(-i * DEG2RAD<float>);

    addStyledPoint(px, py, c);
  }
  return (toAngle - fromAngle) / angleInc + 1;
}

StyledMultiShape2D::~StyledMultiShape2D() {
}

//TODO: Maybe add a different render calls that loops through array of either the render of style
//or the super render
//Shape2D
void StyledMultiShape2D::render() {
  //Get Shader based on style
  Shader* shader = Shader::useShader(GLWin::PER_VERTEX_SHADER);
  shader->setMat4("projection", *(parentCanvas->getProjection()));
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glLineWidth(style->getLineWidth());

  //Draw Solids
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glDrawElements(GL_TRIANGLES, solidIndices.size(), GL_UNSIGNED_INT, 0);

  //Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_LINES, lineIndices.size(), GL_UNSIGNED_INT, 0);

  //Draw Points
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbo);
  glDrawElements(GL_POINTS, pointIndices.size(), GL_UNSIGNED_INT, 0);

  //Unbind
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}
void StyledMultiShape2D::init() {
  //Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //Create VBO for vertices
  //Create an object in the VAO to store all the vertex values
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
  //Desctribe how information is recieved in shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

  //Create SBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as a solid(filled)
  glGenBuffers(1, &sbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * solidIndices.size(), &solidIndices[0], GL_DYNAMIC_DRAW);

  //Create LBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as lines(wireframe)
  glGenBuffers(1, &lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * lineIndices.size(), &lineIndices[0], GL_DYNAMIC_DRAW);

  //temporary bind point size
  //TODO: set input for point size.
  const float pointSize = 10.0f;
  glPointSize(pointSize);

  //Create PBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as points.
  glGenBuffers(1, &pbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * pointIndices.size(), &pointIndices[0], GL_DYNAMIC_DRAW);

  //glGenBuffers(1,&cbo);
  //glBindBuffer(GL_ARRAY_BUFFER,cbo);
  //glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*colors.size(),&colors[0],GL_STATIC_DRAW);
  ////Desctribe how information is recieved in shaders
  //glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);
}

//Solid Primitives
void StyledMultiShape2D::fillRectangle(float x, float y, float w, float h, const glm::vec4& c) {
  uint32_t points = 0;
  addStyledPoint(x, y, c);
  addStyledPoint(x, y + h, c);  // goes in counter-clockwise order
  addStyledPoint(x + w, y + h, c);
  addStyledPoint(x + w, y, c);
  sAddQuadIndices();
  points = 4;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::fillRoundRect(float x, float y, float w, float h, float rx, float ry, const glm::vec4& c) {
  uint32_t points = 0;
  uint32_t cur = getPointIndex();
  uint32_t centerIndex = getPointIndex();
  addStyledPoint(x + w / 2, y + h / 2, c);

  uint32_t toAdd = addSector(x + rx, y + ry, rx, ry, 90, 180, 1, c);
  sAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  solidIndices.push_back(centerIndex);
  solidIndices.push_back(getPointIndex() - 1);
  solidIndices.push_back(getPointIndex());

  toAdd = addSector(x + rx, y + h - ry, rx, ry, 180, 270, 1, c);
  sAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  solidIndices.push_back(centerIndex);
  solidIndices.push_back(getPointIndex() - 1);
  solidIndices.push_back(getPointIndex());

  toAdd = addSector(x + w - rx, y + h - ry, rx, ry, 270, 360, 1, c);
  sAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  solidIndices.push_back(centerIndex);
  solidIndices.push_back(getPointIndex() - 1);
  solidIndices.push_back(getPointIndex());

  toAdd = addSector(x + w - rx, y + ry, rx, ry, 0, 90, 1, c);
  sAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  solidIndices.push_back(centerIndex);
  solidIndices.push_back(getPointIndex() - 1);
  solidIndices.push_back(centerIndex + 1);

  points += 13;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const glm::vec4& c) {
  uint32_t points = 0;
  addStyledPoint(x1, y1, c);
  addStyledPoint(x2, y2, c);
  addStyledPoint(x3, y3, c);
  sAddTriIndices();
  points += 3;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::fillPolygon(float x, float y, float xRad, float yRad, float n, const glm::vec4& c) {
  fillEllipse(x, y, xRad, yRad, 360 / n, c);
}

void StyledMultiShape2D::fillCircle(float x, float y, float rad, float angleInc, const glm::vec4& c) {
  fillEllipse(x, y, rad, rad, angleInc, c);
}

void StyledMultiShape2D::fillEllipse(float x, float y, float xRad, float yRad, float angleInc, const glm::vec4& c) {
  uint32_t points = 0;
  uint32_t cur = getPointIndex();
  uint32_t centerIndex = getPointIndex();
  addStyledPoint(x, y, c);

  uint32_t toAdd = addSector(x, y, xRad, yRad, 0, 360, angleInc, c);
  sAddSectorIndices(centerIndex, toAdd);

  points += 1 + toAdd;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

//Line Primitives
void StyledMultiShape2D::drawRectangle(float x, float y, float w, float h, const glm::vec4& c) {
  uint32_t points = 0;
  uint32_t cur = getPointIndex();
  addStyledPoint(x, y, c);
  addStyledPoint(x, y + h, c);  // goes in counter-clockwise order
  addStyledPoint(x + w, y + h, c);
  addStyledPoint(x + w, y, c);
  lAddQuadIndices();
  points += 4;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::drawRoundRect(float x, float y, float w, float h, float rx, float ry, const glm::vec4& c) {
  uint32_t points = 0;

  uint32_t cur = getPointIndex();
  uint32_t centerIndex = getPointIndex();
  addStyledPoint(x + w / 2, y + h / 2, c);

  uint32_t toAdd = addSector(x + rx, y + ry, rx, ry, 90, 180, 1, c);
  lAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  lineIndices.push_back(getPointIndex() - 1);
  lineIndices.push_back(getPointIndex());

  toAdd = addSector(x + rx, y + h - ry, rx, ry, 180, 270, 1, c);
  lAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  lineIndices.push_back(getPointIndex() - 1);
  lineIndices.push_back(getPointIndex());

  toAdd = addSector(x + w - rx, y + h - ry, rx, ry, 270, 360, 1, c);
  lAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  lineIndices.push_back(getPointIndex() - 1);
  lineIndices.push_back(getPointIndex());

  toAdd = addSector(x + w - rx, y + ry, rx, ry, 0, 90, 1, c);
  lAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  lineIndices.push_back(getPointIndex() - 1);
  lineIndices.push_back(centerIndex + 1);

  points += 9;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const glm::vec4& c) {
  uint32_t points = 0;

  addStyledPoint(x1, y1, c);
  addStyledPoint(x2, y2, c);
  addStyledPoint(x3, y3, c);
  lAddTriIndices();

  points += 3;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::drawPolygon(float x, float y, float xRad, float yRad, float n, const glm::vec4& c) {
  drawEllipse(x, y, xRad, yRad, 360 / n, c);
}

void StyledMultiShape2D::drawCompletePolygon(float x, float y, float xRad, float yRad, float n, const glm::vec4& c) {
}

void StyledMultiShape2D::drawCircle(float x, float y, float rad, float angleInc, const glm::vec4& c) {
  drawEllipse(x, y, rad, rad, angleInc, c);
}

void StyledMultiShape2D::drawEllipse(float x, float y, float xRad, float yRad, float angleInc, const glm::vec4& c) {
  uint32_t points = 0;

  uint32_t cur = getPointIndex();
  uint32_t centerIndex = getPointIndex();
  addStyledPoint(x, y, c);

  uint32_t toAdd = addSector(x, y, xRad, yRad, 0, 360, angleInc, c);
  lAddSectorIndices(centerIndex, toAdd);
  lineIndices.push_back(getPointIndex() - 1);
  lineIndices.push_back(centerIndex + 2);

  points = 1 + toAdd;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::drawLine(float x1, float y1, float x2, float y2, const glm::vec4& c) {
  uint32_t points = 0;
  uint32_t ind = getPointIndex();

  addStyledPoint(x1, y1, c);
  addStyledPoint(x2, y2, c);

  points = 2;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
  lineIndices.push_back(ind++);
  lineIndices.push_back(ind);
}

// rectangular grid
void StyledMultiShape2D::drawGrid(float x0, float y0, float w, float h,
                                  uint32_t numHoriz, uint32_t numVert,
                                  const glm::vec4& c) {
  // draw verticle lines
  float x = x0;
  const float dx = (w / numVert);
  for (
      uint32_t indexVert = numVert + 1; indexVert > 0; indexVert--, x += dx)
    drawLine(x, y0, x, y0 + h, c);
  // draw horizontal lines
  float y = y0;
  const float dy = (h / numHoriz);
  for (
      uint32_t indexHoriz = numHoriz + 1; indexHoriz > 0; indexHoriz--, y += dy)
    drawLine(x0, y, x0 + w, y, c);
}

void StyledMultiShape2D::fillGrid(float x0, float y0, float w, float h,
                                  uint32_t numHoriz, uint32_t numVert,
                                  const glm::vec4& lc, const glm::vec4& bc) {
  // fill background
  fillRectangle(x0, y0, w, h, bc);
  // draw verticle lines
  float x = x0;
  const float dx = (w / numVert);
  for (
      uint32_t indexVert = numVert + 1; indexVert > 0; indexVert--, x += dx)
    drawLine(x, y0, x, y0 + h, lc);
  // draw horizontal lines
  float y = y0;
  const float dy = (h / numHoriz);
  for (
      uint32_t indexHoriz = numHoriz + 1; indexHoriz > 0; indexHoriz--, y += dy)
    drawLine(x0, y, x0 + w, y, lc);
}

// draw an equilateral triangle broken into equilateral triangles
void StyledMultiShape2D::drawTriGrid(float x0, float y0, float s,
                                     uint32_t trianglesPerSide,
                                     const glm::vec4& c) {
  double xmid = x0 + s / 2, side = s / trianglesPerSide;
  double h = side * (sqrt(3) / 2);
  const float yend = y0 + s * (sqrt(3) / 2);
  for (float y = y0, x = x0, len = s; y < yend; y += h, x += s / 2, len -= s)
    drawLine(x, y, x + len, y0, c);
}

void StyledMultiShape2D::drawHexGrid(float x, float y, float w, float h,
                                     uint32_t numHorizHexagons,
                                     const glm::vec4& c) {
}

//drawBezier, end bezier,grid

void StyledMultiShape2D::drawPolyline(const float xy[], uint32_t n, const glm::vec4& c) {
  uint32_t ind = getPointIndex();
  uint32_t j = 0;
  for (uint32_t i = n; i > 0; i--, j += 2)
    addStyledPoint(xy[j], xy[j + 1], c);
  for (uint32_t i = n; i > 1; i--) {
    lineIndices.push_back(ind++);
    lineIndices.push_back(ind);
  }
}

void StyledMultiShape2D::fillPolygon(const float xy[], uint32_t n, const glm::vec4& c) {
  uint32_t ind = getPointIndex();
  uint32_t start = ind;
  uint32_t j = 0;
  double mid_x = 0;
  double mid_y = 0;
  for (uint32_t i = 0; i < n; i++) {
    mid_x += xy[2 * i];
    mid_y += xy[2 * i + 1];
  }
  mid_x /= n;
  mid_y /= n;
  addStyledPoint(mid_x, mid_y, c);
  for (uint32_t i = n; i > 0; i--, j += 2)
    addStyledPoint(xy[j], xy[j + 1], c);
  for (uint32_t i = 0; i < n; i++) {
    solidIndices.push_back(start);
    solidIndices.push_back(ind++);
    solidIndices.push_back(ind);
  }
  solidIndices.push_back(start);
  solidIndices.push_back(ind);
  solidIndices.push_back(start + 1);
}

void StyledMultiShape2D::drawPolygon(const std::vector<float>& xy, const glm::vec4& c) {
  uint32_t ind = getPointIndex();
  uint32_t start = ind;
  if (xy.size() == 0) return;
  for (uint32_t i = 0; i < xy.size(); i += 2)
    addStyledPoint(xy[i], xy[i + 1], c);

  for (uint32_t i = xy.size(); i > 1; i--) {
    lineIndices.push_back(ind++);
    lineIndices.push_back(ind);
  }
  lineIndices.push_back(ind);
  lineIndices.push_back(start);
}

void StyledMultiShape2D::drawPolygon(const float xy[], uint32_t n, const glm::vec4& c) {
  uint32_t ind = getPointIndex();
  uint32_t start = ind;
  uint32_t j = 0;
  for (uint32_t i = n; i > 0; i--, j += 2)
    addStyledPoint(xy[j], xy[j + 1], c);
  for (uint32_t i = n; i > 1; i--) {
    lineIndices.push_back(ind++);
    lineIndices.push_back(ind);
  }
  lineIndices.push_back(ind);
  lineIndices.push_back(start);
}

//Point Primitives
void StyledMultiShape2D::rectanglePoints(float x, float y, float w, float h, const glm::vec4& c) {
  uint32_t points = 0;

  addStyledPoint(x, y, c);
  addStyledPoint(x, y + h, c);  // goes in counter-clockwise order
  addStyledPoint(x + w, y + h, c);
  addStyledPoint(x + w, y, c);
  pAddQuadIndices();

  points = 4;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::roundRectPoints(float x, float y, float w, float h, float rx, float ry, const glm::vec4& c) {
  uint32_t points = 0;
  uint32_t cur = getPointIndex();
  uint32_t centerIndex = getPointIndex();
  addStyledPoint(x + w / 2, y + h / 2, c);

  uint32_t toAdd = addSector(x + rx, y + ry, rx, ry, 90, 180, 1, c);
  pAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  toAdd = addSector(x + rx, y + h - ry, rx, ry, 180, 270, 1, c);
  pAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  toAdd = addSector(x + w - rx, y + h - ry, rx, ry, 270, 360, 1, c);
  pAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  toAdd = addSector(x + w - rx, y + ry, rx, ry, 0, 90, 1, c);
  pAddSectorIndices(centerIndex, toAdd);
  currentIndex += toAdd;
  points += toAdd;

  points += 1;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::trianglePoints(float x1, float y1, float x2, float y2, float x3, float y3, const glm::vec4& c) {
  uint32_t points = 0;

  addStyledPoint(x1, y1, c);
  addStyledPoint(x2, y2, c);
  addStyledPoint(x3, y3, c);
  pAddTriIndices();

  points += 3;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::polygonPoints(float x, float y, float xRad, float yRad, float n, const glm::vec4& c) {
  uint32_t points = 0;
  uint32_t cur = getPointIndex();
  uint32_t centerIndex = getPointIndex();
  addStyledPoint(x, y, c);

  uint32_t toAdd = addSector(x, y, xRad, yRad, 0, 360, 360 / n, c);
  points += toAdd;
  pAddSectorIndices(centerIndex, toAdd);

  points += 1;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::circlePoints(float x, float y, float rad, float angleInc, const glm::vec4& c) {
  uint32_t points = 0;
  uint32_t cur = getPointIndex();
  uint32_t centerIndex = getPointIndex();
  addStyledPoint(x, y, c);

  uint32_t toAdd = addSector(x, y, rad, rad, 0, 360, angleInc, c);
  points += toAdd;
  pAddSectorIndices(centerIndex, toAdd);

  points += 1;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::ellipsePoints(float x, float y, float xRad, float yRad, float angleInc, const glm::vec4& c) {
  uint32_t points = 0;
  uint32_t cur = getPointIndex();
  uint32_t centerIndex = getPointIndex();
  addStyledPoint(x, y, c);

  uint32_t toAdd = addSector(x, y, xRad, yRad, 0, 360, angleInc, c);
  pAddSectorIndices(centerIndex, toAdd);

  points += 1;
  numIndices.push_back(points);
  currentIndex += points;
  startIndices.push_back(currentIndex);
}

void StyledMultiShape2D::updateColors(const uint64_t pos, const float r, const float g, const float b) {
  for (uint32_t i = 0; i < numIndices[pos]; ++i) {
    vertices[startIndices[pos] * 5 + 5 * i + 2] = r;
    vertices[startIndices[pos] * 5 + 5 * i + 3] = g;
    vertices[startIndices[pos] * 5 + 5 * i + 4] = b;
  }
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
}

void StyledMultiShape2D::bezierSegment(const Bezier* b) {
  addStyledPoint(b->getp2x(), b->getp2y(), b->getColor());
  addStyledPoint(b->getp3x(), b->getp3y(), b->getColor());
  uint32_t ind = getPointIndex();
  //    pointIndices.push_back(ind);
  double t = 0;
  for (uint32_t i = 0; i < b->getn(); i++, t += b->getdt()) {
    float x = ((b->getax() * t + b->getbx()) * t + b->getcx()) * t + b->getdx();
    float y = ((b->getay() * t + b->getby()) * t + b->getcy()) * t + b->getdy();
    addStyledPoint(x, y, b->getColor());
    lineIndices.push_back(ind);
    lineIndices.push_back(++ind);
  }
  if (b->getEnd() == true) {
    //        pointIndices.push_back(ind);
    addPoint(b->getp4x(), b->getp4y());
  } else {
    lineIndices.pop_back();
    ind += 2;
    lineIndices.push_back(ind);
    //        pointIndices.push_back(ind);
  }
}

void StyledMultiShape2D::bezierSegmentByPoints(float p1x, float p1y, float p2x, float p2y,
                                               float p3x, float p3y, float p4x, float p4y,
                                               int n, bool end, const glm::vec4& c) {
  float ax = -p1x + 3 * p2x - 3 * p3x + p4x;
  float ay = -p1y + 3 * p2y - 3 * p3y + p4y;
  float bx = 3 * p1x - 6 * p2x + 3 * p3x;
  float by = 3 * p1y - 6 * p2y + 3 * p3y;
  float cx = -3 * p1x + 3 * p2x;
  float cy = -3 * p1y + 3 * p2y;
  Bezier b(ax, bx, cx, p1x, ay, by, cy, p1y, n, end, c);
  bezierSegment(&b);
}

void StyledMultiShape2D::spline(const std::vector<double>& points, int n, const glm::vec4& c) {
  uint32_t num = points.size() / 2 - 1;
  std::vector<double> parameter, dx, dy;

  parameter.push_back(4.0);
  for (uint32_t i = 1; i < num - 1; i++) {
    parameter.push_back(4.0 - 1.0 / parameter[i - 1]);
  }
  parameter.push_back(4.0 - 1.0 / parameter[num - 2]);

  dx.push_back(3 * (points[2 * num] - points[2 * num - 2]) / parameter[num - 1]);
  dy.push_back(3 * (points[2 * num + 1] - points[2 * num - 1]) / parameter[num - 1]);
  for (uint32_t i = num - 2; i > 0; i--) {
    double d = (3 * (points[2 * i + 2] - points[2 * i - 2]) - dx[0]) / parameter[i - 1];
    dx.insert(dx.begin(), d);
    d = (3 * (points[2 * i + 3] - points[2 * i - 1]) - dy[0]) / parameter[i - 1];
    dy.insert(dy.begin(), d);
  }
  dx.insert(dx.begin(), (3 * (points[2] - points[0]) - dx[0]) / parameter[0]);
  dy.insert(dy.begin(), (3 * (points[3] - points[1]) - dy[0]) / parameter[0]);

  for (uint32_t i = 0; i < num; i++) {
    float ax = float(2 * (points[2 * i] - points[2 * i + 2]) + dx[i] + dx[i + 1]);
    float bx = float(3 * (points[2 * i + 2] - points[2 * i]) - 2 * dx[i] - dx[i + 1]);
    float cx = float(dx[i]);
    float dx = float(points[2 * i]);
    float ay = float(2 * (points[2 * i + 1] - points[2 * i + 3]) + dy[i] + dy[i + 1]);
    float by = float(3 * (points[2 * i + 3] - points[2 * i + 1]) - 2 * dy[i] - dy[i + 1]);
    float cy = float(dy[i]);
    float dy = float(points[2 * i + 1]);
    if (i == num - 1) {
      Bezier b(ax, bx, cx, dx, ay, by, cy, dy, n, true, c);
      bezierSegment(&b);
    } else {
      Bezier b(ax, bx, cx, dx, ay, by, cy, dy, n, false, c);
      bezierSegment(&b);
    }
  }
}

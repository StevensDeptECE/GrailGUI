#pragma once

#include <glm/glm.hpp>

#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"

class StyledMultiShape2D : public MultiShape2D {
 public:
  StyledMultiShape2D(Canvas* parent, const Style* s, uint32_t vertCount = 1024,
                     uint32_t solidIndCount = 1024,
                     uint32_t lineIndCount = 1024,
                     uint32_t pointIndCount = 1024)
      : MultiShape2D(parent, s, vertCount, solidIndCount, lineIndCount, pointIndCount,
                     5),
        currentIndex(0) {
    startIndices.push_back(0);
  }

  uint32_t addSector(float x, float y, float xRad, float yRad, float fromAngle,
                     float toAngle, float angleInc, const glm::vec4& c);

  void addColor(const glm::vec4& rgb) {
    vertices.push_back(rgb.x);
    vertices.push_back(rgb.y);
    vertices.push_back(rgb.z);
  }
  void addColor(float r, float g, float b) {
    vertices.push_back(r);
    vertices.push_back(g);
    vertices.push_back(b);
  }
  void setColors(float cols[], uint32_t size) {
    colors.insert(colors.end(), &cols[0], &cols[size]);
  }

  void setColors(std::vector<float>& cols) { colors = cols; }
  void addStyledPoint(float x, float y, const glm::vec4& rgb) {
    addPoint(x, y);
    addColor(rgb);
  }
  // override Shape methods to draw us
  ~StyledMultiShape2D() override;
  void clear() {
    vertices.clear();
    colors.clear();
  }
  void init() override;
  void render() override;

  // Update Methods (in progress)
  void updateColors(const uint64_t pos, const float r, const float g,
                    const float b);

  // Solid Primitives
  void fillRectangle(float x, float y, float w, float h, const glm::vec4& c);
  void fillRoundRect(float x, float y, float w, float h, float rx, float ry,
                     const glm::vec4& c);
  void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3,
                    const glm::vec4& c);
  void fillPolygon(float x, float y, float xRad, float yRad, float n,
                   const glm::vec4& c);
  void fillCircle(float x, float y, float rad, float angleInc,
                  const glm::vec4& c);
  void fillEllipse(float x, float y, float xRad, float yRad, float angleInc,
                   const glm::vec4& c);

  // Line Primitives
  void drawRectangle(float x, float y, float w, float h, const glm::vec4& c);
  void drawRoundRect(float x, float y, float w, float h, float rx, float ry,
                     const glm::vec4& c);
  void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3,
                    const glm::vec4& c);
  void drawPolygon(float x, float y, float xRad, float yRad, float n,
                   const glm::vec4& c);
  void drawCompletePolygon(float x, float y, float xRad, float yRad, float n,
                           const glm::vec4& c);
  void drawCircle(float x, float y, float rad, float angleInc,
                  const glm::vec4& c);
  void drawEllipse(float x, float y, float xRad, float yRad, float angleInc,
                   const glm::vec4& c);
  void drawLine(float x1, float y1, float x2, float y2, const glm::vec4& c);
  void drawPolyline(const float xy[], uint32_t n, const glm::vec4& c);
  void fillPolyline(const float xy[], uint32_t n, const glm::vec4& c);
  void drawPolygon(const float xy[], uint32_t n, const glm::vec4& c);
  void fillPolygon(const float xy[], uint32_t n, const glm::vec4& c);
  // drawBezier, end bezier,grid
  void bezierSegment(const Bezier* b);
  void bezierSegmentByPoints(float p1x, float p1y, float p2x, float p2y,
                             float p3x, float p3y, float p4x, float p4y, int n,
                             bool end, const glm::vec4& c);
  void spline(const std::vector<double>& points, int n, const glm::vec4& c);

  // Grids
  // rectangular grid
  void drawGrid(float x0, float y0, float w, float h, uint32_t numHoriz,
                uint32_t numVert, const glm::vec4& c);

  void fillGrid(float x0, float y0, float w, float h, uint32_t numHoriz,
                uint32_t numVert, const glm::vec4& lc, const glm::vec4& bc);

  // draw an equilateral triangle broken into equilateral triangles
  void drawTriGrid(float x, float y, float s, uint32_t trianglesPerSide,
                   const glm::vec4& c);
  void drawHexGrid(float x, float y, float w, float h,
                   uint32_t numHorizHexagons, const glm::vec4& c);

  // Point Primitives
  void rectanglePoints(float x, float y, float w, float h, const glm::vec4& c);
  void roundRectPoints(float x, float y, float w, float h, float rx, float ry,
                       const glm::vec4& c);
  void trianglePoints(float x1, float y1, float x2, float y2, float x3,
                      float y3, const glm::vec4& c);
  void polygonPoints(float x, float y, float xRad, float yRad, float n,
                     const glm::vec4& c);
  void circlePoints(float x, float y, float rad, float angleInc,
                    const glm::vec4& c);
  void ellipsePoints(float x, float y, float xRad, float yRad, float angleInc,
                     const glm::vec4& c);
 private:
  std::vector<float> colors;
  std::vector<int> startIndices;
  std::vector<int> numIndices;
  int currentIndex;
};

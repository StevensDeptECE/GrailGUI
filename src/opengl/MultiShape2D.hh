#pragma once

#include "opengl/Bezier.hh"
#include "opengl/MultiShape.hh"

class Style;
class MultiShape2D : public MultiShape {
 protected:
  const Style* style;
  constexpr static uint32_t elemPerVert = 2;

  uint32_t addSector(float x, float y, float xRad, float yRad, float fromAngle,
                     float toAngle, float angleInc);
  uint32_t getPointIndex() const {
    return MultiShape::getPointIndex(elemPerVert);
  }
  void addLine(float x1, float y1, float x2, float y2) {
    uint32_t ind = getPointIndex();

    addPoint(x1, y1);
    addPoint(x2, y2);
    lineIndices.push_back(ind++);
    lineIndices.push_back(ind);
  }

 public:
  void drawPoint(float x, float y);
  MultiShape2D(Canvas* parent, const Style* s, uint32_t lineWidth = 1,
               uint32_t vertCount = 1024, uint32_t solidIndCount = 1024,
               uint32_t lineIndCount = 1024, uint32_t pointIndCount = 1024,
               uint32_t elemPerVert = 2)
      : MultiShape(parent, lineWidth, vertCount, solidIndCount, lineIndCount,
                   pointIndCount),
        style(s) {}
  ~MultiShape2D();

  // override Shape methods to draw us
  void init() override;
  void render() override;

  // Solid Primitives
  void fillRectangle(float x, float y, float w, float h);
  void fillRoundRect(float x, float y, float w, float h, float rx, float ry);
  void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
  void fillPolygon(float x, float y, float xRad, float yRad, float n);
  void fillCircle(float x, float y, float rad, float angleInc);
  void fillEllipse(float x, float y, float xRad, float yRad, float angleInc);

  // Line Primitives
  void drawRectangle(float x, float y, float w, float h);
  void drawRoundRect(float x, float y, float w, float h, float rx, float ry);
  void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
  void drawPolygon(float x, float y, float xRad, float yRad, float n);
  void drawCompletePolygon(float x, float y, float xRad, float yRad, float n);
  void drawCircle(float x, float y, float rad, float angleInc);
  void drawEllipse(float x, float y, float xRad, float yRad, float angleInc);
  void drawLine(float x1, float y1, float x2, float y2);

  // drawBezier, end bezier,grid
  void bezierSegment(const Bezier* b);
  void bezierSegmentByPoints(float p1x, float p1y, float p2x, float p2y,
                             float p3x, float p3y, float p4x, float p4y, int n,
                             bool end);
  void endBezierSegment(const Bezier* b);
  void spline(const std::vector<double>& points, int n);

  // Point Primitives
  void rectanglePoints(float x, float y, float w, float h);
  void roundRectPoints(float x, float y, float w, float h, float rx, float ry);
  void trianglePoints(float x1, float y1, float x2, float y2, float x3,
                      float y3);
  void polygonPoints(float x, float y, float xRad, float yRad, float n);
  void circlePoints(float x, float y, float rad, float angleInc);
  void ellipsePoints(float x, float y, float xRad, float yRad, float angleInc);

  // getter for style
  const Style* getStyle();
};

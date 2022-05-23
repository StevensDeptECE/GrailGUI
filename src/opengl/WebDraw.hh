#pragma once

#include <glm/glm.hpp>

#include "WebCursor.hh"
#include "opengl/Canvas.hh"
#include "opengl/primitivesV2/MultiShape2D.hh"
#include "opengl/primitivesV2/StyledMultiShape2D.hh"
#include "util/Buffer.hh"
#include "util/DynArray.hh"

/*
  WebDraw is a single class that manages OpenGL to draw many kinds of 2d shapes
  in different colors and fonts accordingly, every method has a parameter for
  Style from which color and font are taken
 */
class WebDraw : public MultiShape2D {
 private:
  DynArray<float> colors;
  DynArray<int> startIndices;
  DynArray<int> numIndices;
  int currentIndex;

 public:
  WebDraw(uint32_t vertCount = 1024, uint32_t solidIndCount = 1024,
          uint32_t lineIndCount = 1024, uint32_t pointIndCount = 1024)
      : MultiShape2D(s, vertCount, solidIndCount, lineIndCount, pointIndCount,
                     5),
        currentIndex(0) {
    startIndices.push_back(0);
  }

  uint32_t addSector(float x, float y, float xRad, float yRad, float fromAngle,
                     float toAngle, float angleInc, const Style* s);

  // add a color to the list
  // TODO: vec4 would contain alpha, so this should probably push that in
  void addColor(const glm::vec4& rgb) {
    colors.add(rgb.r);
    colors.add(rgb.g);
    colors.add(rgb.b);
    colors.add(rgb.a);
  }

  // add a color as 3 floats setting opacity to 1
  void addColor(float r, float g, float b) {
    colors.add(r);
    colors.add(g);
    colors.add(b);
    colors.add(1);  // full opaque
  }

  // TODO: What is this? looks like adding a list of colors to the end
  // TODO: poorly named at the very least
  void setColors(float cols[], uint32_t size) {
    colors.insert(colors.end(), &cols[0], &cols[size]);
  }

  // Replace the current list of colors with a new vector
  // TODO: this is probably not something that should be supported
  void setColors(std::vector<float>& cols) { colors = cols; }

  // Add a 2d point, and add its color at the same time
  void addStyledPoint(float x, float y, const glm::vec4& rgb) {
    addPoint(x, y);
    addColor(rgb);
  }

  // Every shape must be able to set up in advance (init)
  void init() override;

  /*
   *Every shape must be able to draw itself
   * this will draw all content in the WebDraw including
   * first draw all polygons with a different color for each vertex
   */
  void render() override;

  // Update Methods (in progress)
  void updateColors(const uint64_t pos, const float r, const float g,
                    const float b);

  // Solid Primitives
  void fillRectangle(float x, float y, float w, float h, const Style* s);
  void fillRoundRect(float x, float y, float w, float h, float rx, float ry,
                     const Style* s);
  void fillTriangle(float x1, float y1, float x2, float y2, float x3, float y3,
                    const Style* s);
  void fillPolygon(float x, float y, float xRad, float yRad, float n,
                   const Style* s);
  // TODO: probably get rid of angleInc, but figure out how to control the
  // accuracy
  void fillCircle(float x, float y, float rad, float angleInc, const Style* s);
  void fillEllipse(float x, float y, float xRad, float yRad, float angleInc,
                   const Style* s);

  // Line Primitives
  void drawRectangle(float x, float y, float w, float h, const Style* s);
  void drawRoundRect(float x, float y, float w, float h, float rx, float ry,
                     const Style* s);
  void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3,
                    const Style* s);
  void drawPolygon(float x, float y, float xRad, float yRad, float n,
                   const Style* s);
  void drawCompletePolygon(float x, float y, float xRad, float yRad, float n,
                           const Style* s);
  void drawCircle(float x, float y, float rad, float angleInc, const Style* s);
  void drawEllipse(float x, float y, float xRad, float yRad, float angleInc,
                   const Style* s);
  void drawLine(float x1, float y1, float x2, float y2, const Style* s);

  // draw a section of a circle, angles are in radians
  void drawArc(float x, float y, float r, float ang1, float ang2);

  /*
                TODO: this should support drawBezier and drawSpline
                evaluate the 2d cubic where
                a = xy[i], b = xy[i+1], c = xy[i+2], d = xy[i+3]
    e = xy[i+4], f = xu[i+5], g = xy[i+6], h = xy[i+7]
                so that

                x(t) = (((a*t + b)*t + c)*t + d  (Horner's form)
                y(t) = (((e*t + f)*t + g)*t + h  (Horner's form)

                The vector of coefficients must still be generated. For Bezier,
                this can be done directly from each 4 control points. For
    Splines, this requires solving a system of linear equations see
    http://mathworld.wolfram.com/Spline.html

                numSegments is the number of lines to draw per cubic piece
        */
  void drawCubic(float xy[], uint32_t length, uint32_t numSegments);

  // TODO: How do we take Bezier and Spline objects and generate the underlying
  // xy[] coefficients to drive the above?

  // Point Primitives
  void rectanglePoints(float x, float y, float w, float h, const Style* s);
  void roundRectPoints(float x, float y, float w, float h, float rx, float ry,
                       const Style* s);
  void trianglePoints(float x1, float y1, float x2, float y2, float x3,
                      float y3, const Style* s);
  void polygonPoints(float x, float y, float xRad, float yRad, float n,
                     const Style* s);
  void circlePoints(float x, float y, float rad, float angleInc,
                    const Style* s);
  void ellipsePoints(float x, float y, float xRad, float yRad, float angleInc,
                     const Style* s);

  // draw a rectangular region of a texture to a rectangle
  void drawImage(float x, float y,     // location on the screen to draw
                 float w, float h,     // size of rectangle on the screen
                 uint32_t textureId,   // the texture to draw to this location
                 float u1, float v1,   // one corner of the texture to render
                 float u2, float v2);  // the other corner of the texture

  // TODO: load texture!!! load from png, FLIF, etc.

  // draw an entire rectangular text to a rectangle
  void drawImage(float x, float y,  // location on the screen to draw
                 float w, float h,  // size of rectangle on the screen
                 uint32_t textureId) {
    drawImage(x, y, w, h, textureId, 0, 0, 1, 1);
  }

  void drawText(float x, float y, Style* s, const char text[], uint32_t len);

  // draw characters starting at baseChar in some character set. For example
  // Greek characters start at 0x3d0 in Unicode, so express relative to that.
  void drawText(float x, float y, Style* s, uint32_t baseChar,
                const char text[], uint32_t len);
  // TODO: unicode characters for multibyte?
  void drawText(float x, float y, Style* s, const uint16_t text[],
                uint32_t len);

  // TODO: how to support different text directions?
  // see: https://developer.mozilla.org/en-US/docs/Web/CSS/text-combine-upright

  // size of text width given string (works with prop font)
  float getTextWidth(Style* s, const char text[], uint32_t len) const;
  float getTextHeight(Style* s);

  // draw text at the current insert point, if it does not fit move to next
  // line, but if it doesn't
  // TODO: we need font metrics!!
  void addText(WebCursor* cursor, Style* s, const char text[], uint32_t len) {
    // bounding box = height, need to find width
    // call moveRight to update cursor position based on font size and lenght of
    // array
    cout << "Adding text..\n";
    for (size_t i = 0; i < len; i++) {
      Glyph* gly = s->f->getGlyph(text[i]);

      // draw the glyph

      // advance cursor
      cursor->moveRight(gly->advance, f->getHeight());
    }
  };

#if 0
  // add support for sequences of addText...
  //TODO: probably should be done using a generic REPEAT tag
  void addText(WebCursor* cursor, Style*s[], const);


  //TODO: can we remove all WebCursor* from the parameters, and just have a single WebCursor in the WebDraw object?
	void addImage(WebCursor* cursor, Style* s, float w, float h,
								uint32_t textureId, float u1, float v1, float u2, float v2);
	void addImage(WebCursor* cursor, Style* s, float w, float h,
								uint32_t textureId) {
    addImage(cursor, s, w,h, textureId, 0,0, 1,1);
  }
  
  //TODO: What other drawing primitives do we put inline?
  //TODO: add for speed, subset of uint64_t	void drawNum(uint32_t v);
  //TODO: same	void drawNum(int32_t v);
	void drawNum(uint64_t v);
  void drawNum(int64_t v);
               void drawNum(int32_t v);
               //TODO: will work without this just a bit slower	void drawNum(float v);
	void drawNum(double v);

  // TODO: spec out what this actually does
  // 1.2345678
  // 1.2345e+72
  // What to do if number is out of range?
	void addNum(WebCursor* cursor, double v, int precision);
	void drawNum(float x, float y, uint32_t v);
	void drawNum(float x, float y, int32_t v);
	void drawNum(float x, float y, uint64_t v);
	void drawNum(float x, float y, uint32_t v);
	void drawNum(float x, float y, double v, uint32_t precision);
	void drawExponential(float x, float y, double v, int precision);

  //TODO: Think about this, it's very cool
  // draw number starting at x,y moving along angle ang in radians
  void drawNumAtAngle(float x, float y, uint64_t v, float ang);
  void drawTextAtAngle(float x, float y, uint64_t v, float ang);

  /*
    draw number starting at x,y moving along curve
    CubicFunc will need arclength of each segment. 
    1. Compute the length of the number in digits
    2. calculate the total arclength of cubic
    3. set spacing accordingly

    TODO: even weirder, would we like to add a function that would allow character angles following different function?
  */
  void drawNumAlongCurve(float x, float y, uint64_t v, CubicFunc& cubic, bool charAngleFollowCurve);

  // same for arbitrary text
  void drawTextAlongCurve(float x, float y, const char text[], uint32_t len,
                          CubicFunc& cubic, bool charAngleFollowCurve);

  // TODO: no need to do right justified numbers separately if we write an efficient routine
	void drawNumRightJust(float x, float y, uint32_t v);
	void drawNumRightJust(float x, float y, int32_t v);
	void drawNumRightJust(float x, float y, uint64_t v);
	void drawNumRightJust(float x, float y, uint32_t v);
	void drawNumRightJust(float x, float y, double v, uint32_t precision);

  
  // read from the metadata, pull whatever is specified from the data and draw
  void draw(Buffer& metadata, Buffer& data);
#endif
};

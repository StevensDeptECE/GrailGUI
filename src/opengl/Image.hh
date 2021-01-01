#pragma once

#include "opengl/Shape.hh"
#include <cstdint>
#include <iostream>
#include "opengl/util/Transformation.hh"

class GLWin;
class Style;
class Image : public Shape{
private:
  float x,y, rWidth,rHeight;
  int tWidth,tHeight,bpp;
  unsigned char * data;
  uint32_t textureID, vao;
  std::vector<float> vertices;
  std::vector<uint32_t> indices;
  Style * style;
  Transformation * transform; 
  //void addVertices();
public:
  Image(Canvas* c, float x, float y, float width, float height, uint32_t textureId, Style * s);
  Image(Canvas* c, float x, float y, float width, float height, const char * filePath, Style * s);

  void init();
  void process_input(Inputs* in,float dt) {}
  void update() {}
  void render();
  /*
     add a subrectangle of this image to be drawn at (x,y,width,height)
     using the rectangle in the image at texture coordinates (u0,v0, u1,v1)
     */
  void addImage(float x, float y, float width, float height, float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);
  static void combineImage(const std::vector<std::string> &images);
};

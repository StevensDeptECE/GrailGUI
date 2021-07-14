#pragma once

#include <cstdint>

#include "opengl/Shape.hh"

class Image : public Shape {
 private:
  // x, y, width, and height of what we will actually draw
  float x, y, width, height;
  // variables that stb_load will put values into
  int textureWidth, textureHeight, nrChannels;
  // place to store the data we get from stb_load
  unsigned char *data;
  uint32_t textureID;
  std::vector<float> vertices;
  std::vector<uint32_t> indices;
  void setupBuffers(float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);
  void setupTexture();

 public:
  Image(Canvas *c, float x, float y, float width, float height,
        uint32_t textureID = 1);
  Image(Canvas *c, float x, float y, float width, float height,
        const char *filePath, uint32_t textureID = 1);

  void addImage(const char *filePath);

  void init();
  void update() {}
  void render();
  // static void combineImage(const std::vector<std::string> &images);
};

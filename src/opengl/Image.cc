#include "opengl/Image.hh"

#include <string>

#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "util/Ex.hh"
using namespace std;

Image::Image(Canvas *c, float x, float y, float width, float height,
             uint32_t textureID)
    : Shape(c),
      x(x),
      y(y),
      drawWidth(width),
      drawHeight(height),
      textureID(textureID) {
  vertices.reserve(16);
  setupBuffers(x, y, width, height);
  setupTexture();
}

Image::Image(Canvas *c, float x, float y, float width, float height,
             const char *filePath, uint32_t textureID)
    : Shape(c),
      x(x),
      y(y),
      drawWidth(width),
      drawHeight(height),
      textureID(textureID) {
  vertices.reserve(16);
  setupBuffers(x, y, width, height);
  setupTexture();
  addImage(filePath);
}

void Image::setupBuffers(float u0, float v0, float u1, float v1) {
  uint32_t offset = vertices.size() / 4;

  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(u0);
  vertices.push_back(v1);

  vertices.push_back(x);
  vertices.push_back(y + drawHeight);
  vertices.push_back(u0);
  vertices.push_back(v0);

  vertices.push_back(x + drawWidth);
  vertices.push_back(y + drawHeight);
  vertices.push_back(u1);
  vertices.push_back(v0);

  vertices.push_back(x + drawWidth);
  vertices.push_back(y);
  vertices.push_back(u1);
  vertices.push_back(v1);

  indices.push_back(offset);
  indices.push_back(offset + 1);
  indices.push_back(offset + 2);
  indices.push_back(offset);
  indices.push_back(offset + 2);
  indices.push_back(offset + 3);
}

void Image::setupTexture() {
  // glGenTextures takes an input for how many textures we want to make, and
  // stored them in an unsigned int array, given as the second argument (we just
  // do a single unsigned int)
  glGenTextures(1, &textureID);
  // we need to bind the newly created object so future texture commands will
  // target this one
  glBindTexture(GL_TEXTURE_2D, textureID);

  // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Image::addImage(const char *filePath) {
  // stbi_load take a filepath as input, and outputs values to textureWidth,
  // textureheight, and nrChannels
  data = stbi_load(filePath, &textureWidth, &textureHeight, &nrChannels, 0);
  if (data) {
    // after these calls, the currently bound texture object will have the
    // texture image attached to it
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw Ex2(Errcode::IMAGE_LOAD, filePath);
  }
  // we don't need this data any more, its being held by opengl, so we can free
  // what we've got
  stbi_image_free(data);
}

void Image::init() {
  // Creating rect VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0],
               GL_STATIC_DRAW);

  // Index array object
  glGenBuffers(1, &sbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
               &indices[0], GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
}

void Image::render() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  Shader *s = Shader::useShader(GLWin::TEXTURE_SHADER);
  s->setMat4("projection", *(parentCanvas->getProjection()));
  s->setInt("ourTexture", 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glDrawElements(GL_TRIANGLES, vertices.size() * 3 / 8, GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

#if 0
void Image::combineImage(const vector<string> &images) {
  int w, h, c;
  int w_total = 0, h_total = 0;
  std::vector<unsigned char> combinedImageData;
  combinedImageData.reserve(images.size() * 256 * 256);
  for (int i = 0; i < images.size(); i++) {
    string file = images.at(i);
    unsigned char *img = stbi_load(file.c_str(), &w, &h, &c, STBI_rgb_alpha);
    if (w > w_total) w_total = w;
    h_total += h;
    const uint32_t image_size = w * h * c;
    if (combinedImageData.capacity() < image_size + combinedImageData.size())
      combinedImageData.reserve(2 *
                                (combinedImageData.capacity() + image_size));
    combinedImageData.insert(combinedImageData.end(), img, img + image_size);
    stbi_image_free(img);
    ////combinedImageData = combinedImageData + imageData.at(i);
  }
  stbi_write_jpg("test.jpg", w_total, h_total, STBI_rgb_alpha,
                 combinedImageData.data(), 100);
}
#endif

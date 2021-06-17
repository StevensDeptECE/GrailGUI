#include "opengl/Image.hh"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <string>

#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"
#include "opengl/Style.hh"
#include "stb/stb_image_write.h"
#include "util/Ex.hh"
using namespace std;

Image::Image(Canvas* c, float x, float y, float width, float height,
             uint32_t textureId, Style* s)
    : Shape(c),
      x(x),
      y(y),
      rWidth(width),
      rHeight(height),
      textureID(textureId),
      style(s) {
  vertices.reserve(16);
  addImage(x, y, width, height);
}

Image::Image(Canvas* c, float x, float y, float width, float height,
             const char* filePath, Style* s)
    : Shape(c), x(x), y(y), rWidth(width), rHeight(height), style(s) {
  stbi_set_flip_vertically_on_load(
      true);  // tell stb_image.h to flip loaded texture's on the y-axis.
  data = stbi_load(filePath, &tWidth, &tHeight, &bpp, 0);
  if (!data) {
    throw Ex2(Errcode::IMAGE_LOAD, filePath);
  }

  textureID = 0;
  addImage(x, y, width, height);
}

//? Adds cropped sections of image
void Image::addImage(float x, float y, float width, float height, float u0,
                     float v0, float u1, float v1) {
  uint32_t offset = vertices.size() / 4;

  vertices.push_back(x);
  vertices.push_back(y);
  vertices.push_back(u0);
  vertices.push_back(v1);

  vertices.push_back(x);
  vertices.push_back(y + height);
  vertices.push_back(u0);
  vertices.push_back(v0);

  vertices.push_back(x + width);
  vertices.push_back(y + height);
  vertices.push_back(u1);
  vertices.push_back(v0);

  vertices.push_back(x + width);
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

void Image::init() {
  // Choosing between RGB and RGBA
  GLint internal = (bpp == 3) ? GL_RGB : GL_RGBA;
  GLint format;
  switch (bpp) {
    case 1:
      format = GL_RED;
      break;
    case 2:
      format = GL_RG;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      format = GL_RGB;
      break;
  }

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
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void*)(2 * sizeof(float)));

  // Add Texture if this was not already done
  if (textureID == 0) {
    // Generate Texture object
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (bpp == 1) {
      GLint swiz[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
      glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swiz);
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internal, tWidth, tHeight, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  }
}

void Image::render() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  Shader* s = Shader::useShader(GLWin::TEXTURE_SHADER);
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

void Image::combineImage(const vector<string>& images) {
  int w, h, c;
  int w_total = 0, h_total = 0;
  std::vector<unsigned char> combinedImageData;
  combinedImageData.reserve(images.size() * 256 * 256);
  for (int i = 0; i < images.size(); i++) {
    string file = images.at(i);
    unsigned char* img = stbi_load(file.c_str(), &w, &h, &c, STBI_rgb_alpha);
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

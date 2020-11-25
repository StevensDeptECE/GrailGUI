#include "TextureArray.hh"

GLuint TextureArray::loadImage(const char* imagePath, uint8_t unit) {
  if (!(unit >= 0 && unit < allocSize)) {
    std::cerr << "Texture unit must be 0-" + std::to_string(allocSize - 1)
              << std::endl;
    return 0;
  }

  // Bind and activate texture
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, textureID[unit]);

  // Load from STBI
  int width, height, numComponents;
  stbi_set_flip_vertically_on_load(
      true);  // tell stb_image.h to flip loaded texture's on the y-axis.
  unsigned char* data =
      stbi_load(imagePath, &width, &height, &numComponents, 4);
  if (data == NULL) {
    std::cerr << "Texture loading failed for texture: " << imagePath
              << std::endl;
    return 0;
  }

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  stbi_image_free(data);

  // Filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return 1;
}

GLuint TextureArray::bindImage(uint8_t unit) {
  if (!(unit >= 0 && unit <= allocSize)) {
    std::cerr << "Texture unit must be 0-" + std::to_string(allocSize - 1)
              << std::endl;
    return 0;
  }

  shader->setInt("ourTexture[" + std::to_string(unit) + "]", unit);

  // Bind and activate texture
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, textureID[unit]);
  return 1;  // 1= success,
             // TODO: Make 0=success
}

TextureArray::TextureArray(Shader* shader, uint8_t allocSize)
    : shader(shader), allocSize(allocSize) {
  if (!(allocSize >= 1 && allocSize <= 32)) {
    std::cerr << "Texture alloc size must be 1-32" << std::endl;
  }

  textureID[allocSize];
  glGenTextures(allocSize, textureID);
}

TextureArray::~TextureArray() { glDeleteTextures(allocSize, textureID); }
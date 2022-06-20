#pragma once

#include <cstdint>
#include <string>

#include "opengl/Shape.hh"

/**
 * @brief
 *
 */
class Image : public Shape {
 private:
  float x, y, width, height;
  int textureWidth, textureHeight,
      nrChannels; /**< Variables that stb_load will put values into */
  unsigned char *data /**< Where stb_load will store the data it loads */;
  uint32_t textureID; /**< ID associated with the texture we will create */
  std::vector<float> vertices;
  std::vector<uint32_t> indices;

  /**
   * @brief Set up the vertices and indices buffers
   *
   * @param u0
   * @param v0
   * @param u1
   * @param v1
   */
  void setupBuffers(float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);

  /**
   * @brief Set up the texture object for this image.
   *
   */
  void setupTexture();

 public:
  /**
   * @brief Construct a new Image object
   *
   * @param c Pointer to a Canvas.
   * @param x X coord of the top left corner.
   * @param y Y coord of the top left corner.
   * @param width Width of the drawn image in pixels.
   * @param height Height of the drawn image in pixels
   * @param textureID A desired ID for the texture, don't use 0.
   */
  Image(Canvas *c, float x, float y, float width, float height,
        uint32_t textureID = 1);

  /**
   * @brief Construct a new Image object
   *
   * @param c Pointer to a Canvas.
   * @param x X coord of the top left corner.
   * @param y Y coord of the top left corner.
   * @param width Width of the drawn image in pixels.
   * @param height Height of the drawn image in pixels
   * @param filePath Path to the image to load.
   * @param textureID A desired ID for the texture, don't use 0.
   */
  Image(Canvas *c, float x, float y, float width, float height,
        const char *filePath, uint32_t textureID = 1);

  /**
   * @brief Add an image to the object. This will overwrite a previously added
   * image.
   *
   * @param filePath Path to the image to load.
   */
  void addImage(const char *filePath);

  /**
   * @brief OpenGL setup that will be called by GLWin.
   *
   */
  void init() override;

  /**
   * @brief Artifact of subclassing from Shape, this is not used.
   *
   */
  void update() override {}

  /**
   * @brief Renders the image, this will be called by GLWin.
   *
   */
  void render(glm::mat4& trans) override;
};

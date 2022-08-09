#pragma once

#include <cstdint>
#include <string>

#include "glad/glad.h"
#include "opengl/Shape.hh"

/**
 * @brief
 *
 */
class Image : public Shape {
 private:
  float x, y, width, height;
  uint32_t vbo;  // handle to the point data on the graphics card
  // TODO: get rid of sbo and just darw the whole vbo array (2 triangles)
  int textureWidth, textureHeight,
      nrChannels;      /**< Variables that stb_load will put values into */
  uint32_t textureID;  /**< ID associated with the texture we will create */
  float vertices[16];  // x,y,u,v, x,y,u,v
  /**
   * @brief Set up the rectangle to be drawn within the image
   *
   * @param u0
   * @param v0
   * @param u1
   * @param v1
   */
  void setRect(float u0 = 0, float v0 = 0, float u1 = 1, float v1 = 1);

  /**
   * @brief Set up the texture object for this image.
   *
   */
  void setupTexture();

 public:
  /**
   * @brief Construct a new Image object from an existing texture
   *
   * @param x coord of the top left corner.
   * @param y coord of the top left corner.
   * @param width of the drawn image in pixels.
   * @param height of the drawn image in pixels
   * @param textureID A desired ID for the texture, don't use 0.
   */
  Image(Canvas* c, float x, float y, float width, float height,
        uint32_t textureID = 1);

  /**
   * @brief Construct a new Image object
   *
   * @param x coord of the top left corner.
   * @param y coord of the top left corner.
   * @param width Width of the drawn image in pixels.
   * @param height Height of the drawn image in pixels
   * @param filePath Path to the image to load.
   */
  Image(Canvas* c, float x, float y, float width, float height,
        const char* filePath);

  /**
   * @brief set the image of the object.
   * This will overwrite the previous image.
   *
   * @param filePath Path to the image to load.
   */
  void setImage(const char filePath[]);

  /**
   * @brief OpenGL setup that will be called by GLWin to load the image.
   *
   */
  void init() override;

  /**
   * @brief Renders the image to the screen, called by the parent tab
   *
   */
  void render(glm::mat4& trans) override;

  /**
   * @brief If the image is to be modified, update may be written
   *
   */
  void update() override;

  void setupBuffers(float u0, float v0, float u1, float v1);

  void cleanup() { glDeleteTextures(1, &textureID); }

  ~Image() { cleanup(); }
};

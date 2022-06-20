#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#if 0
namespace std {
  template<typename T>
    class vector;
};
#endif

class Inputs;
class Canvas;
class GLWin;
class Shape {
 protected:
  uint32_t vao, vbo, sbo, lbo, pbo, cbo;
  Canvas* parentCanvas;
  template <typename T>
  void gen(uint32_t& vb, std::vector<T>& list);
  template <typename T>
  void gen(uint32_t& vb, std::vector<T>& list, uint32_t stride, uint32_t shaderParam, uint32_t numElements, uint32_t offset);
  template <typename T>
  void gen(uint32_t& vb, std::vector<T>& list, uint32_t stride,
           uint32_t shaderParam1, uint32_t numElements1, uint32_t offset1,
           uint32_t shaderParam2, uint32_t numElements2, uint32_t offset2);
 public:
  Shape(Canvas* parent) : parentCanvas(parent) {}
  virtual ~Shape();

  /**
   * @brief Get a pointer to the GLWin object
   *
   * @return GLWin* Window containing the Shape
   */
  GLWin* getWin() const;
#if 0
  void setParentCanvas(Canvas *c) {
    parentCanvas = c;
  }
#endif
#if 0
  // Get the width, in pixels, of this Shape's
  // parent Canvas
  uint32_t getParentCanvasWidth()
  {
    if (parentCanvas)
      return parentCanvas->getWidth();
    return 0;
  }

  // Get the height, in pixels, of this Shape's
  // parent Canvas
  uint32_t getParentCanvasHeight()
  {
    if (parentCanvas)
      return parentCanvas->getHeight();
    return 0;
  }
#endif

  virtual void init() = 0;
  // TODO: We need to clean up and eliminate process_input or completely
  // overhaul in the meantime, I just made the parent implement a default nop
  virtual void process_input(Inputs* in, float dt) {}
  virtual void update() = 0;
  virtual void render(glm::mat4& trans) = 0;
};

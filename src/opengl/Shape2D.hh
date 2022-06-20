#pragma once

#include <vector>
//#include <iostream>
#include "opengl/Shader.hh"
#include "opengl/Shape.hh"
#include "opengl/Style.hh"
#include "opengl/util/Transformation.hh"

class Shape2D : public Shape {
 protected:
  float x, y;

  std::vector<float> vertices;
  std::vector<float> colors;  // move this to style
  std::vector<uint32_t> solidIndices;
  std::vector<uint32_t> lineIndices;
  std::vector<uint32_t> pointIndices;

  const Style* style;
  Transformation* transform;

  void applyTransform(Shader* s);

 public:
  // Shape2D(float x, float y) : Shape2D(x,y,nullptr,nullptr){}
  // Shape2D(float x, float y, Transformation* transform) :
  // Shape2D(x,y,nullptr,transform){}
  Shape2D(Canvas* c, float x, float y, const Style* style,
          Transformation* transform = new Transformation())
      : Shape(c), x(x), y(y), style(style), transform(transform) {}

  void addPoint(float x, float y) {
    vertices.push_back(x);
    vertices.push_back(y);
  }
  void addColor(float r, float g, float b) {
    colors.push_back(r);
    colors.push_back(g);
    colors.push_back(b);
  }
  void setColors(float cols[], uint32_t size) {
    colors.insert(colors.end(), &cols[0], &cols[size]);
  }

  void setColors(std::vector<float> cols) { colors = cols; }
  std::vector<float> getVertices() { return vertices; }

  uint32_t getSize() { return vertices.size(); }

  const Style* getStyle() const { return style; }

  Transformation* getTransformation() { return transform; }
  void setTransform(Transformation* t) { transform = t; }
  ~Shape2D() override;
  virtual void init() override;
  virtual void process_input(Inputs* in, float dt) override {}
  virtual void update() override {}
  virtual void render(glm::mat4& trans) = 0;
  void initIndices();
};

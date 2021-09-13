#pragma once

#include "opengl/Shape.hh"
#include <glm.hpp>
class GUIWidget : public Shape {
  protected:
  float x, y, w, h; // bounds of the widget
  float padX, padY;
  const Shader* shader;
public:
  GUIWidget(Canvas* c, float x, float y, float w, float h, float padX = 5, float padY = 5) :
   Shape(c), x(x), y(y), w(w), h(h), padX(padX), padY(padY) {
   }
  void init() override;
  void setPadX(float x) { padX = x; }
  void setPadY(float y) { padY = y; }
  void drawRect(float x, float y, float w, float h, const glm::vec4& color); 
  void fillRect(float x, float y, float w, float h, const glm::vec4& color); 
  void drawCircle(float x, float y, float r, const glm::vec4& color); 
  void fillCircle(float x, float y, float r, const glm::vec4& color); 
  void text(float x, float y, const char t[], uint32_t len, const glm::vec4& color);
};
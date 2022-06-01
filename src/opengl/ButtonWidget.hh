#pragma once

#include <functional>
#include <optional>
#include <string>

#include "opengl/InteractiveWidget2D.hh"

class ButtonWidget : public InteractiveWidget2D {
 private:
  std::string text;
  std::optional<std::function<void(void)>> func;

 public:
  ButtonWidget(MainCanvas* c, const Style* s, float x, float y, float w,
               float h, const std::string& text, const char action[]);
  ButtonWidget(StyledMultiShape2D* m, MultiText* t, const Style* s, float x,
               float y, float w, float h, const std::string& text,
               const char action[]);
  ButtonWidget(MainCanvas* c, const Style* s, float x, float y,
               const std::string& text, const char action[]);
  void click(float mouseX, float mouseY) override;
  void init() override;

  void redraw();

  template <typename Func>
  void setAction(Func func) {
    this->func = func;
  }

  void updateCanvas(MainCanvas* canvas) { c = canvas; };

  float getX() { return x; };
  float getY() { return y; };
  float getW() { return w; };
  float getH() { return h; };
};
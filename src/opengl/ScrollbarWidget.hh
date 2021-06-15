#include <string>

#include "glm/glm.hpp"
#include "opengl/Colors.hh"
#include "opengl/StyledMultiShape2D.hh"

class ScrollbarWidget : public StyledMultiShape2D {
 private:
  float x, y, w, h;
  float scrollbarBoxY;
  float boxSize;
  glm::vec4 bgColor;

 public:
  ScrollbarWidget(Canvas* c, const Style* s, float x, float y, float w, float h)
      : StyledMultiShape2D(c, s),
        bgColor(glm::vec4(0, 0, 1, 0.7)),
        x(x),
        y(y),
        scrollbarBoxY(y + 3),
        w(w),
        h(h),
        boxSize(h / 2) {}
  // void ;
  void init() override;
  void render() override;
  void update() override;
  void draw();
  void scroll(float dy);
};
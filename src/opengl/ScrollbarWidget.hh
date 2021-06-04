#include <string>

#include "opengl/Widget2D.hh"

class ScrollbarWidget : public Widget2D {
 private:
  std::string text;

 public:
  ScrollbarWidget(StyledMultiShape2D* m, MultiText* t, float x, float y,
                  float w, float h)
      : Widget2D(m, t, x, y, w, h) {}
  // void ;
  void init() override;
};
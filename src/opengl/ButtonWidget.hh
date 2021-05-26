#include "opengl/Widget2D.hh"
#include <string>

class ButtonWidget : public Widget2D {
private:
  std::string text;
public:
  ButtonWidget(StyledMultiShape2D* m, MultiText* t, const std::string& text,
               float x, float y, float w, float h) : Widget2D(m,t, x, y, w, h), text(text) {}
  void init() override;
};
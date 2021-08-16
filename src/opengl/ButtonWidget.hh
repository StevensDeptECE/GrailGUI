#include <functional>
#include <string>

#include "opengl/InteractiveWidget2D.hh"

class ButtonWidget : public InteractiveWidget2D {
 private:
  std::string text;
  std::function<void(void)> func;

 public:
  void click(float mouseX, float mouseY) override;
  ButtonWidget(MainCanvas* c, float x, float y, float w, float h,
               const std::string& text, const char action[])
      : InteractiveWidget2D(c, x, y, w, h), text(text) {
    int borderSize = 2;

    m->fillRectangle(x - borderSize, y - borderSize, w + (borderSize * 2),
                     h + (borderSize * 2), grail::black);
    m->fillRectangle(x, y, w, h, grail::red);
    t->addCentered(x, y, w, h, c->getStyle()->f, text);
    // lookup action in GLWin and doit!
  }
  void init() override;

  template <typename Func>
  void setAction(Func func) {
    this->func = func;
  }
};
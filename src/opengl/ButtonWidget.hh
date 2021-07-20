#include <string>

#include "opengl/InteractiveWidget2D.hh"

class ButtonWidget : public InteractiveWidget2D {
 private:
  std::string text;

 public:
  ButtonWidget(MainCanvas* c, float x, float y, float w, float h,
               const std::string& text, const char action[])
      : InteractiveWidget2D(c, x, y, w, h), text(text) {
    int borderSize = 2;

    m->fillRectangle(x - borderSize, y - borderSize, w + (borderSize * 2),
                     h + (borderSize * 2), grail::black);
    m->fillRectangle(x, y, w, h, grail::red);
    t->add(x + (w / 2), y + (h / 2), text.c_str(), text.length());
    // lookup action in GLWin and doit!
  }
  void init() override;
  void click(float xPress, float yPress, float xRelease,
             float yRelease) override;
};
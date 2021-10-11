#include <functional>
#include <optional>
#include <string>

#include "opengl/InteractiveWidget2D.hh"

class ButtonWidget : public InteractiveWidget2D {
 private:
  std::string text;
  std::optional<std::function<void(void)>> func;

 public:
  ButtonWidget(MainCanvas* c, float x, float y, float w, float h,
               const std::string& text, const char action[]);
  void click(float mouseX, float mouseY) override;
  void init() override;

  void redraw();

  template <typename Func>
  void setAction(Func func) {
    this->func = func;
  }
};
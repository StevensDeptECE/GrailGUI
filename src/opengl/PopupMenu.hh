#pragma once

#include <string>

#include "opengl/Widget2D.hh"
#include "util/DynArray.hh"

class PopupMenu : public Widget2D {
 private:
  DynArray<std::string> menuItems;
  float padX, padY;
  const Font* menuFont;

 public:
  PopupMenu(StyledMultiShape2D* m, MultiText* t, Font* f, float x, float y,
            float w, float h)
      : Widget2D(m, t, x, y, w, h), menuItems(16), menuFont(f) {}

  PopupMenu(StyledMultiShape2D* m, MultiText* t, const Font* f, float x,
            float y, float w, float h, const std::string menuItems[],
            uint32_t len)
      : Widget2D(m, t, x, y, w, h), menuItems(len), menuFont(f) {
    for (uint32_t i = 0; i < len; i++) {
      this->menuItems.add(menuItems[i]);
    }
  }
  void setPadX(float x) { padX = x; }
  void setPadY(float y) { padY = y; }
  void init() override;
};

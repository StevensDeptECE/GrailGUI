#include "opengl/PopupMenu.hh"

#include "opengl/Colors.hh"

void PopupMenu::init() {
  m->drawRectangle(x, y, w, h, grail::black);
  float x = this->x + padX, y = this->y + padY + menuFont->getHeight();
  for (uint32_t i = 0; i < menuItems.size(); i++, y += menuFont->getHeight())
    t->add(x, y, menuFont, menuItems[i]);
}

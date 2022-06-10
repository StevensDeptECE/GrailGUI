#include "opengl/DropDownMenu.hh"

#include <iostream>

DropDownMenu::DropDownMenu(GLWin* w, float x, float y, float menuWidth,
                           float menuHeight, const char label[],
                           float axisPadding, Style* buttonStyle,
                           bool isVertical)
    : Member(w->getSharedMenuTab(), "DDMenu", 0),
      w(w),
      buttonStyle(buttonStyle),
      isVertical(isVertical) {
  menuButton = new ButtonWidget(tab->getMainCanvas(), buttonStyle, x, y, label,
                                "open menu");
  menuButton->setAction([this]() { this->openClose(); });

  if (isVertical) {
    nav = new NavigationBar(w, x, y + menuButton->getH(), menuWidth, menuHeight,
                            axisPadding, isVertical);
  } else {
    nav = new NavigationBar(w, x + menuButton->getW(), y, menuWidth, menuHeight,
                            axisPadding, isVertical);
  }
  nav->setButtonStyle(buttonStyle->f, buttonStyle->bg, buttonStyle->fg,
                      buttonStyle->lineWidth);

  // closeMenu();
  isOpen = true;
}

// TODO: fix menu breaking when isReverseOrder is true
DropDownMenu::DropDownMenu(GLWin* w, float x, float y, const char label[],
                           float axisPadding, float menuHorizontalPadding,
                           float menuVerticalPadding, Style* buttonStyle,
                           bool isVertical, bool isReverseOrder)
    : Member(w->getSharedMenuTab(), "DDMenu", 0),
      w(w),
      buttonStyle(buttonStyle),
      isVertical(isVertical) {
  menuButton = new ButtonWidget(tab->getMainCanvas(), buttonStyle, x, y, label,
                                "open menu");
  menuButton->setAction([this]() { this->openClose(); });

  if (isVertical) {
    nav = new AutoNavBar(w, x, y + menuButton->getH(), axisPadding,
                         menuHorizontalPadding, menuVerticalPadding, isVertical,
                         isReverseOrder);
  } else {
    nav = new AutoNavBar(w, x + menuButton->getW(), y, axisPadding,
                         menuHorizontalPadding, menuVerticalPadding, isVertical,
                         isReverseOrder);
  }
  nav->setButtonStyle(buttonStyle->f, buttonStyle->bg, buttonStyle->fg,
                      buttonStyle->lineWidth);

  // closeMenu();
  isOpen = true;
}

void DropDownMenu::openMenu() {
  std::cout << "Opening menu!" << std::endl;
  isOpen = true;
}
void DropDownMenu::closeMenu() {
  std::cout << "Closing menu!" << std::endl;
  isOpen = false;
  /*
  MainCanvas* c = w->getSharedTab()->getMainCanvas();
  c->getGui()->clear();
  c->getGuiText()->clear();
  */
}

/*
void DropDownMenu::setButtonStyle(const Font* f, glm::vec4 borderColor,
                                  glm::vec4 buttonColor,
                                  float borderThickness) {
  buttonStyle->f = f;
  buttonStyle->bg = borderColor;
  buttonStyle->fg = buttonColor;
  buttonStyle->lineWidth = borderThickness;

  c->getGuiText()->clear();
  menuButton->redraw();
  nav->setButtonStyle(f, borderColor, buttonColor, borderThickness);
}
*/
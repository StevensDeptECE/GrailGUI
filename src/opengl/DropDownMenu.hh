#pragma once

#include "opengl/AutoNavBar.hh"

class DropDownMenu : public Member {
 private:
  NavigationBar* nav;
  ButtonWidget* menuButton;
  GLWin* w;
  bool isVertical, isOpen;
  Style* buttonStyle;

 public:
  DropDownMenu(GLWin* w, float x, float y, float menuWidth, float menuHeight,
               const char label[], float axisPadding,
               Style* buttonStyle = new Style(FontFace::get("TIMES", 20,
                                                            FontFace::BOLD),
                                              grail::black, grail::red, 1),
               bool isVertical = true);
  DropDownMenu(GLWin* w, float x, float y, const char label[],
               float axisPadding, float menuHorizontalPadding,
               float menuVerticalPadding,
               Style* buttonStyle = new Style(FontFace::get("TIMES", 20,
                                                            FontFace::BOLD),
                                              grail::black, grail::red, 1),
               bool isVertical = true, bool isReverseOrder = false);
  void openMenu();
  void closeMenu();
  void openClose() {
    if (isOpen) {
      closeMenu();
    } else {
      openMenu();
    }
  };
  /*
  void setButtonStyle(const Font* f, glm::vec4 borderColor,
                      glm::vec4 buttonColor, float borderThickness);
  */
};
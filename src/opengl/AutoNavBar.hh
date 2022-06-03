#pragma once

#include "opengl/NavigationBar.hh"

/**
 * @brief Version of NavigationBar in which new buttons are automatically added
 * when a new member is added. The dimensions of the buttons and the navigation
 * bar are also automatically determined and updated.
 *
 */
class AutoNavBar : public NavigationBar {
 private:
  bool isReverseOrder;
  float horizontalPadding, verticalPadding;
  float currentX, currentY;

 public:
  AutoNavBar(GLWin* w, float x, float y, float axisPadding,
             float horizontalPadding, float verticalPadding,
             bool isVertical = false, bool isReverseOrder = false);

  void addButton(Member* m);
  void advancePosition();
};
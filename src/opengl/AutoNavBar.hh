#pragma once

#include "opengl/NavigationBar.hh"

/**
 * @brief Version of NavigationBar in which new buttons are automatically added
 * when a new member is added. The dimensions of the buttons and the navigation
 * bar are also automatically determined and updated. An AutoNavBar should be
 * added to a window BEFORE any other members.
 *
 */
class AutoNavBar : public NavigationBar {
 private:
  bool isReverseOrder;
  float horizontalPadding, verticalPadding;

 public:
  /**
   * @brief Create a new automated navigation bar
   *
   * @param w The initial window to draw on
   * @param x x-coordinate of top-left corner
   * @param y y-coordinate of top-left corner
   * @param axisPadding distance between the edges of two buttons
   * @param horizontalPadding horizontal distance between buttons and bar edge
   * @param verticalPadding vertical distance between buttons and bar edge
   * @param isVertical allow vertical navigation bars (e.x. Ubuntu's task bar)
   * @param isReverseOrder display buttons in the reverse order the members are
   * added (leftmost/topmost button corresponds to first member displayed)
   */
  AutoNavBar(GLWin* w, float x, float y, float axisPadding,
             float horizontalPadding, float verticalPadding,
             bool isVertical = false, bool isReverseOrder = false);

  /**
   * @brief Add a button to the nav bar for a new member
   *
   * @param m pointer to member
   */
  void addButton(Member* m);

  /**
   * @brief Update the positions of all buttons in the nav bar such that their
   * displayed left-to-right/top-to-bottom order is opposite that of their index
   * order
   *
   */
  void reverseButtonOrder();

  /**
   * @brief Updates the dimensions of the nav bar to fit around the buttons.
   * Option to add padding between the buttons and nav bar edges. Override of
   * fitBarDimensions function from NavigationBar that accounts for
   * isReverseOrder
   *
   * @param widthPadding horizontal distance between nav bar edge and closest
   * button
   * @param heightPadding vertical distance between nav bar edge and closest
   * button
   */
  void fitBarDimensions(float widthPadding = 0, float heightPadding = 0);

  /**
   * @brief Calculates and returns the lengthwise distance of the list of
   * buttons in the nav bar. Override of buttonsLength function from
   * NavigationBar that accounts for isReverseOrder
   */
  float buttonsLength();
};
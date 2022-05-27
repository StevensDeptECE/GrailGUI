#pragma once

#include <iostream>
#include <unordered_set>

#include "opengl/ButtonWidget.hh"

class NavigationBar : public Member {
 private:
  std::vector<ButtonWidget*> buttons;
  std::unordered_set<MainCanvas*> canvases;
  GLWin* parentWin;
  MainCanvas* currentCanvas;
  StyledMultiShape2D m;
  MultiText t;

  float defaultButtonWidth;
  float defaultButtonHeight;
  float defaultEdgeWidth;

  Style* buttonStyle;
  float xPos, yPos, barWidth, barHeight, axisPadding;
  bool isVertical;

  /**
   * @brief Convenience constructor so getMainCanvas isn't called so often
   *
   * @param initialCanvas The initial canvas to draw on
   * @param initialStyle The default style from the canvas
   * @param x x-coordinate of top-left corner
   * @param y y-coordinate of top-left corner
   * @param width width of navigation bar
   * @param height height of navigation bar
   * @param axisPadding distance between the edges of two buttons
   * @param isVertical allow vertical navigation bars (e.x. Ubuntu's task bar)
   */
  NavigationBar(MainCanvas* initialCanvas, const Style* initialStyle, float x,
                float y, float width, float height, float axisPadding,
                bool isVertical);

 public:
  /**
   * @brief Create a new (horizontal or vertical) navigation bar
   *
   * @param w The initial window to draw on
   * @param x x-coordinate of top-left corner
   * @param y y-coordinate of top-left corner
   * @param width width of navigation bar
   * @param height height of navigation bar
   * @param axisPadding distance between the edges of two buttons
   * @param isVertical allow vertical navigation bars (e.x. Ubuntu's task bar)
   */
  NavigationBar(GLWin* w, float x, float y, float width, float height,
                float axisPadding, bool isVertical = false);

  /**
   * @brief Create a new button and return a reference to it
   *
   * @param width width of the new button
   * @param height height of the new button
   * @param label the text to be shown on the button
   * @param action a description of the button's action
   * @return ButtonWidget& a reference to the new button
   */
  ButtonWidget* addButton(float width, float height, std::string label,
                          const char action[]);

  /**
   * @brief Automatically calculate axis offset for new button on nav bar
   *
   */
  float axisOffset();

  /**
   * @brief Set an action for a button in the nav bar
   *
   * @param buttonIndex index of button in buttons vector
   * @param func function call for action
   */
  void setButtonAction(int buttonIndex,
                       std::optional<std::function<void(void)>> func);

  /**
   * @brief Balance n buttons on the nav bar
   *
   * This is particularly useful in scenarios where a screen can fit n tabs
   * and there are m tabs that could be displayed (n < m). In this case, the
   * NavigationBar will determine the ideal placement of each button such
   * that at most numButtons will be displayed at a given time.
   *
   * @param numButtons number of buttons to display
   */
  void balanceButtons(int numButtons);

  /**
   * @brief Adds a new tab to the window and adds the navigation bar to the tab
   *
   */
  void addNewTab();

  void addToTab(Tab* tab);

  /*
  void render() {
    std::cout << "printing from NavigationBar::render()" << std::endl;
  }
  */
};
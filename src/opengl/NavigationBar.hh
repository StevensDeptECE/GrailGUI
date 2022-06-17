#pragma once

#include <iostream>
#include <unordered_set>

#include "opengl/ButtonWidget.hh"

class NavigationBar : public Member {
 protected:
  std::vector<ButtonWidget*> buttons;
  GLWin* parentWin;
  MainCanvas* c;

  float defaultButtonWidth;
  float defaultButtonHeight;
  float defaultEdgeWidth;

  Style* buttonStyle;
  Style* barStyle;
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
  NavigationBar(MainCanvas* initialCanvas, float x, float y, float width,
                float height, float axisPadding, bool isVertical);

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
   * @brief Create a new button and return a pointer to it
   *
   * @param width width of the new button
   * @param height height of the new button
   * @param label the text to be shown on the button
   * @param action a description of the button's action
   * @return ButtonWidget* a pointer to the new button
   */
  ButtonWidget* addButton(float width, float height, std::string label,
                          const char action[]);

  /**
   * @brief Create a new button and return a pointer to it. Button
   * width/height are automatically determined based on the text and buttonStyle
   * font
   *
   * @param label the text to be shown on the button
   * @param action a description of the button's action
   * @return ButtonWidget* a pointer to the new button
   */
  ButtonWidget* addButton(std::string label, const char action[]);

  /**
   * @brief Automatically calculate and return the axis offset for new button on
   * nav bar
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
   * @brief Redefine the style of all buttons in the nav bar
   *
   * @param f font of button text
   * @param borderColor color of button border
   * @param buttonColor main color of button
   * @param borderThickness thickness of button border
   */
  void setButtonStyle(const Font* f, glm::vec4 borderColor,
                      glm::vec4 buttonColor, float borderThickness);

  /**
   * @brief Redefine the style for the nav bar box
   *
   * @param borderColor color of box border
   * @param barColor main color of box
   * @param borderThickness thickness of box border
   */
  void setBarStyle(glm::vec4 borderColor, glm::vec4 barColor,
                   float borderThickness);

  /**
   * @brief Draw a box around the dimension of the nav bar
   *
   */
  void drawBarBox();

  /**
   * @brief Updates the dimensions of the nav bar to fit around the buttons.
   * Option to add padding between the buttons and nav bar edges
   *
   * @param widthPadding horizontal distance between nav bar edge and closest
   * button
   * @param heightPadding vertical distance between nav bar edge and closest
   * button
   */
  void fitBarDimensions(float widthPadding = 0, float heightPadding = 0);

  /**
   * @brief Calculates and returns the lengthwise distance of the list of
   * buttons in the nav bar
   */
  float buttonsLength();

  /**
   * @brief Calculates and returns the crosswise distance of the list of
   * buttons in the nav bar
   */
  float buttonsWidth();

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

  MainCanvas* getCanvas() { return c; };

  /**
   * @brief Adds a new tab to the window and adds the navigation bar to the tab
   *
   *
  /*
  void addNewTab();

  void addToTab(Tab* tab);
  void render() {
    std::cout << "printing from NavigationBar::render()" << std::endl;
  }
  */
};
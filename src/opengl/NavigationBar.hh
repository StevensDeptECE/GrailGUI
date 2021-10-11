#include <unordered_set>

#include "opengl/ButtonWidget.hh"

class NavigationBar {
 private:
  std::vector<ButtonWidget> buttons;
  std::unordered_set<MainCanvas*> canvases;
  StyledMultiShape2D m;
  MultiText t;

 public:
  /**
   * @brief Create a new (horizontal or vertical) navigation bar
   *
   * @param initialCanvas The initial canvas to draw on
   * @param x x-coordinate of top-left corner
   * @param y y-coordinate of top-left corner
   * @param width width of navigation bar
   * @param height height of navigation bar
   * @param axisPadding distance between the edges of two buttons
   * @param isVertical allow vertical navigation bars (e.x. Ubuntu's task bar)
   */
  NavigationBar(MainCanvas* initialCanvas, float x, float y, float width,
                float height, float axisPadding, bool isVertical = false);

  /**
   * @brief Create a new button and return a reference to it
   *
   * @param width width of the new button
   * @param height height of the new button
   * @param axisOffset offset relative to the previous button
   * @return ButtonWidget& a reference to the new button
   */
  ButtonWidget& addButton(float width, float height, float axisOffset);

  /**
   * @brief Balance n buttons on the nav bar
   *
   * This is particularly useful in scenarios where a screen can fit n tabs and
   * there are m tabs that could be displayed (n < m). In this case, the
   * NavigationBar will determine the ideal placement of each button such that
   * at most numButtons will be displayed at a given time.
   *
   * @param numButtons number of buttons to display
   */
  void balanceButtons(int numButtons);

  /**
   * @brief Allows the buttons to be reparented to a new MainCanvas
   *
   * This function uses the fact that each InteractiveWidget2D, and therefore
   * each Widget2D can use a custom StyledMultiShape2D and MultiText to swap
   * over to a new MainCanvas and redraw each shape.
   *
   * @param c
   */
  void reparentButtons(MainCanvas* c);
};
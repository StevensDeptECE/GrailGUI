#pragma once
#include "opengl/GrailGUI.hh"

class StyledMultiShape2D;
class MultiText;

/**
 * @brief Represents a Widget that creates StyledMultiShape2D and MultiTexts to
 * draw itself.
 *
 */
class SuperWidget2D {
 protected:
  Canvas* c;
  float x, y; /**< Coords of the top left corner of the widget */
  float w, h; /**< Width and height of the widget */
 public:
  /**
   * @brief Construct a new Super Widget 2 D object
   *
   * @param c Pointer to a canvas
   * @param x X location of top left corner
   * @param y Y location of top left corner
   * @param w Width of the widget
   * @param h Height of the widget
   */
  SuperWidget2D(Canvas* c, float x, float y, float w, float h)
      : c(c), x(x), y(y), w(w), h(h) {}

  // TODO: Should init be called something else based on what it's doing?
  /**
   * @brief Draws the widget using it's created primitive objects.
   *
   */
  virtual void init() = 0;
};

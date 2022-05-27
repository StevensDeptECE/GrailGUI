#include "opengl/NavigationBar.hh"

#include <algorithm>
#include <functional>

#include "util/TypeAlias.hh"

using namespace std;

// TODO: make this confirm to some design guide, maybe use material's design
// guide on tabs?

NavigationBar::NavigationBar(MainCanvas* initialCanvas,
                             const Style* initialStyle, float x, float y,
                             float width, float height, float axisPadding,
                             bool isVertical)
    : Member(initialCanvas->getTab()),
      currentCanvas(initialCanvas),
      m(initialCanvas, initialStyle),
      t(initialCanvas, initialStyle),
      xPos(x),
      yPos(y),
      barWidth(width),
      barHeight(height),
      axisPadding(axisPadding),
      isVertical(isVertical) {
  canvases.insert(initialCanvas);

  this->isVertical = isVertical;

  defaultButtonHeight = min(48_f32, height);
  defaultButtonWidth = min(90_f32, width);
  defaultEdgeWidth = min(16_f32, width / 3);
  /*
  addButton(defaultButtonWidth, defaultButtonHeight, axisPadding, "+",
            "add tab")
      .setAction(bind(&NavigationBar::addTab, this));
  */
}

NavigationBar::NavigationBar(GLWin* w, float x, float y, float width,
                             float height, float axisPadding, bool isVertical)
    : NavigationBar(w->getSharedTab()->getMainCanvas(),
                    w->getSharedTab()->getMainCanvas()->getStyle(), x, y, width,
                    height, axisPadding, isVertical) {
  parentWin = w;
}

ButtonWidget* NavigationBar::addButton(float width, float height, string label,
                                       const char action[]) {
  if (isVertical) {
    buttons.push_back(new ButtonWidget(currentCanvas, xPos, yPos + axisOffset(),
                                       width, height, label, action));
  } else {
    buttons.push_back(new ButtonWidget(currentCanvas, xPos + axisOffset(), yPos,
                                       width, height, label, action));
  }
  return buttons.back();
}

float NavigationBar::axisOffset() {
  if (buttons.empty()) {
    return 0;
  } else {
    if (isVertical) {
      return buttons.back()->getY() + buttons.back()->getH() + axisPadding;
    } else {
      return buttons.back()->getX() + buttons.back()->getW() + axisPadding;
    }
  }
}

void NavigationBar::setButtonAction(
    int buttonIndex, std::optional<std::function<void(void)>> func) {
  buttons[buttonIndex]->setAction(func);
}

void NavigationBar::addNewTab() {
  Tab* t = parentWin->addTab();
  canvases.insert(t->getMainCanvas());
  parentWin->toLastTab();
  // addToTab(t);
}

/*
void NavigationBar::addToTab(Tab* tab) {
  tab->addMember(this);
  MainCanvas* newCanvas = tab->getMainCanvas();
  canvases.insert(newCanvas);
  newCanvas->copy(*currentCanvas);
  tab->setUpdate();
  tab->setRender();
}
*/

void NavigationBar::balanceButtons(int numButtons) {}
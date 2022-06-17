#include "opengl/NavigationBar.hh"

#include <algorithm>
#include <functional>

#include "util/TypeAlias.hh"

using namespace std;

// TODO: make this confirm to some design guide, maybe use material's design
// guide on tabs?

NavigationBar::NavigationBar(MainCanvas* initialCanvas, float x, float y,
                             float width, float height, float axisPadding,
                             bool isVertical)
    : Member(initialCanvas->getTab(), "NavBar", 0),
      c(initialCanvas),
      parentWin(initialCanvas->getWin()),
      xPos(x),
      yPos(y),
      barWidth(width),
      barHeight(height),
      axisPadding(axisPadding),
      isVertical(isVertical) {
  buttonStyle =
      new Style(parentWin->getMenuFont(), grail::black, grail::red, 1);
  barStyle = new Style(parentWin->getMenuFont(), grail::black, grail::gray, 1);

  /*
  defaultButtonHeight = min(48_f32, height);
  defaultButtonWidth = min(90_f32, width);
  defaultEdgeWidth = min(16_f32, width / 3);

  addButton(defaultButtonWidth, defaultButtonHeight, axisPadding, "+",
              "add tab")
        .setAction(bind(&NavigationBar::addTab, this));
    */
}

NavigationBar::NavigationBar(GLWin* w, float x, float y, float width,
                             float height, float axisPadding, bool isVertical)
    : NavigationBar(w->getSharedTab()->getMainCanvas(), x, y, width, height,
                    axisPadding, isVertical) {}

ButtonWidget* NavigationBar::addButton(float width, float height, string label,
                                       const char action[]) {
  if (isVertical) {
    buttons.push_back(new ButtonWidget(c, buttonStyle, xPos,
                                       yPos + axisOffset(), width, height,
                                       label, action));
  } else {
    buttons.push_back(new ButtonWidget(c, buttonStyle, xPos + axisOffset(),
                                       yPos, width, height, label, action));
  }
  return buttons.back();
}

ButtonWidget* NavigationBar::addButton(string label, const char action[]) {
  if (isVertical) {
    buttons.push_back(new ButtonWidget(c, buttonStyle, xPos,
                                       yPos + axisOffset(), label, action));
  } else {
    buttons.push_back(new ButtonWidget(c, buttonStyle, xPos + axisOffset(),
                                       yPos, label, action));
  }
  return buttons.back();
}

float NavigationBar::axisOffset() {
  if (buttons.empty()) {
    return 0;
  }

  if (isVertical) {
    return buttons.back()->getY() + buttons.back()->getH() + axisPadding;
  } else {
    return buttons.back()->getX() + buttons.back()->getW() + axisPadding;
  }
}

void NavigationBar::setButtonAction(
    int buttonIndex, std::optional<std::function<void(void)>> func) {
  buttons[buttonIndex]->setAction(func);
}

void NavigationBar::setButtonStyle(const Font* f, glm::vec4 borderColor,
                                   glm::vec4 buttonColor,
                                   float borderThickness) {
  buttonStyle->f = f;
  buttonStyle->bg = borderColor;
  buttonStyle->fg = buttonColor;
  buttonStyle->lineWidth = borderThickness;
}

void NavigationBar::setBarStyle(glm::vec4 borderColor, glm::vec4 barColor,
                                float borderThickness) {
  barStyle->bg = borderColor;
  barStyle->fg = barColor;
  barStyle->lineWidth = borderThickness;
}

void NavigationBar::drawBarBox() {
  StyledMultiShape2D* m = c->getGui();
  float borderSize = barStyle->lineWidth;
  m->fillRectangle(xPos - borderSize, yPos - borderSize,
                   barWidth + (borderSize * 2), barHeight + (borderSize * 2),
                   barStyle->bg);
  m->fillRectangle(xPos, yPos, barWidth, barHeight, barStyle->fg);

  for (ButtonWidget* b : buttons) {
    b->redraw();
  }
}

void NavigationBar::fitBarDimensions(float widthPadding, float heightPadding) {
  if (isVertical) {
    barWidth = buttonsWidth() + widthPadding;
    barHeight = buttonsLength() + heightPadding;
  } else {
    barWidth = buttonsLength() + widthPadding;
    barHeight = buttonsWidth() + heightPadding;
  }
}

float NavigationBar::buttonsLength() {
  if (isVertical) {
    return buttons.back()->getY() + buttons.back()->getH() -
           buttons.front()->getY();
  } else {
    return buttons.back()->getX() + buttons.back()->getW() -
           buttons.front()->getX();
  }
}

float NavigationBar::buttonsWidth() {
  float width;
  if (isVertical) {
    width = buttons[0]->getW();
    for (ButtonWidget* b : buttons) {
      width = max(b->getW(), width);
    }

  } else {
    width = buttons[0]->getH();
    for (ButtonWidget* b : buttons) {
      width = max(b->getH(), width);
    }
  }
  return width;
}

/*
void NavigationBar::addNewTab() {
  Tab* t = parentWin->addTab();
  // canvases.insert(t->getMainCanvas());
  //  addToTab(t);
}

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
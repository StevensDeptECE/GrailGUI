#include "opengl/AutoNavBar.hh"

using namespace std;

AutoNavBar::AutoNavBar(GLWin* w, float x, float y, float axisPadding,
                       float horizontalPadding, float verticalPadding,
                       bool isVertical, bool isReverseOrder)
    : NavigationBar(w, x, y, 0, 0, axisPadding, isVertical),
      horizontalPadding(horizontalPadding),
      verticalPadding(verticalPadding),
      isReverseOrder(isReverseOrder) {
  w->autoNavBar = this;
}

void AutoNavBar::addButton(Member* m) {
  int index = buttons.size();
  ButtonWidget* bw = NavigationBar::addButton(m->getName(), "action");
  bw->setAction(bind(&GLWin::switchTab, parentWin, index));

  if (isReverseOrder) {
    reverseButtonOrder();
    c->getGuiText()->clear();
  }

  fitBarDimensions(horizontalPadding, verticalPadding);
  drawBarBox();
}

void AutoNavBar::reverseButtonOrder() {
  float currentX = xPos;
  float currentY = yPos;
  for (int i = buttons.size() - 1; i >= 0; i--) {
    if (isVertical) {
      buttons[i]->setY(currentY);
      currentY += buttons[i]->getH() + axisPadding;
    } else {
      buttons[i]->setX(currentX);
      currentX += buttons[i]->getW() + axisPadding;
    }
  }
}

void AutoNavBar::fitBarDimensions(float widthPadding, float heightPadding) {
  if (isVertical) {
    barWidth = buttons[0]->getW();
    for (ButtonWidget* b : buttons) {
      barWidth = max(b->getW(), barWidth);
    }

    if (isReverseOrder) {
      barHeight = buttons.front()->getY() + buttons.front()->getH() -
                  buttons.back()->getY();
    } else {
      barHeight = buttons.back()->getY() + buttons.back()->getH() -
                  buttons.front()->getY();
    }
  } else {
    if (isReverseOrder) {
      barWidth = buttons.front()->getX() + buttons.front()->getW() -
                 buttons.back()->getX();
    } else {
      barWidth = buttons.back()->getX() + buttons.back()->getW() -
                 buttons.front()->getX();
    }

    barHeight = buttons[0]->getH();
    for (ButtonWidget* b : buttons) {
      barHeight = max(b->getH(), barHeight);
    }
  }

  barWidth += widthPadding;
  barHeight += heightPadding;
}
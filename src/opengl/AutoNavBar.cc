#include "opengl/AutoNavBar.hh"

using namespace std;

AutoNavBar::AutoNavBar(GLWin* w, float x, float y, float axisPadding,
                       float horizontalPadding, float verticalPadding,
                       bool isVertical, bool isReverseOrder)
    : NavigationBar(w, x, y, 0, 0, axisPadding, isVertical),
      currentX(x),
      currentY(y),
      horizontalPadding(horizontalPadding),
      verticalPadding(verticalPadding),
      isReverseOrder(isReverseOrder) {
  w->autoNavBar = this;
}

/*
  1. how to add left-to-right, right-to-left automatically


  2. how to set button actions properly

*/
void AutoNavBar::addButton(Member* m) {
  int index = buttons.size();
  ButtonWidget* bw;
  if (isVertical) {
    bw = new ButtonWidget(c, buttonStyle, xPos, yPos + axisOffset(),
                          m->getName(), "action");
  } else {
    bw = new ButtonWidget(c, buttonStyle, xPos + axisOffset(), yPos,
                          m->getName(), "action");
  }
  buttons.push_back(bw);
  bw->setAction(bind(&GLWin::switchTab, parentWin, index));

#if 0

  if (isReverseOrder) {
    if (isVertical) {
      // TODO: New positioning algorithm for when isLeftToRight is true
      // axisOffset() will not work
      buttons.insert(buttons.begin(),
                     bw = new ButtonWidget(c, buttonStyle, xPos, yPos + axisOffset(),
                                      m->getName(), "action"));
    } else {
      buttons.insert(buttons.begin(),
                     bw = new ButtonWidget(c, buttonStyle, xPos + axisOffset(), yPos,
                                      m->getName(), "action"));
    }
    updateButtonPositions();
  } else {
    if (isVertical) {
      buttons.push_back(bw = new ButtonWidget(
          c, buttonStyle, xPos, yPos + axisOffset(), m->getName(), "action"));
    } else {
      buttons.push_back(bw = new ButtonWidget(c, buttonStyle, xPos + axisOffset(),
                                         yPos, m->getName(), "action"));
    }
  }

  int index = buttons.size() - 1; // get the index of the current button so we can activate the corresponding tab
  bw->setAction(bind(GLWin::switchTab(index), parentWin));
#endif
  fitBarDimensions(horizontalPadding, verticalPadding);
  drawBarBox();
}

void AutoNavBar::advancePosition() {
  if (isVertical) {
    currentY += axisOffset();
  } else {
    currentX += axisOffset();
  }
}
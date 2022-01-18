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

  addButton(defaultButtonWidth, defaultButtonHeight, axisPadding, "+",
            "add tab")
      .setAction(bind(&NavigationBar::addTab, this));
}

NavigationBar::NavigationBar(Tab* initialTab, float x, float y, float width,
                             float height, float axisPadding, bool isVertical)
    : NavigationBar{initialTab->getMainCanvas(),
                    initialTab->getMainCanvas()->getStyle(),
                    x,
                    y,
                    width,
                    height,
                    axisPadding,
                    isVertical} {
  parentWin = initialTab->getParentWin();
}

ButtonWidget& NavigationBar::addButton(float width, float height,
                                       float axisOffset, string label,
                                       const char action[]) {
  buttons.emplace_back(currentCanvas, xPos, yPos, width, height, label, action);
  buttons.back().setStyledMultiShape(&m);
  buttons.back().setMultiText(&t);
  buttons.back().redraw();
  return buttons.back();
}

void NavigationBar::addTab() {
  Tab* t = parentWin->addTab();
  t->addMember(this);
  reparentButtons(t->getMainCanvas());
  t->setUpdate();
  t->setRender();
}

void NavigationBar::reparentButtons(MainCanvas* c) {
  if (canvases.find(c) == canvases.end()) {
    c->addLayer(&m);
    c->addLayer(&t);
    canvases.insert(c);
  }
  for (ButtonWidget& b : buttons) {
    c->addClickableWidget(&b);
  }
}

void NavigationBar::balanceButtons(int numButtons) {}
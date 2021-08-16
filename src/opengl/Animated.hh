#pragma once

class Tab;
class MainCanvas;

/**
 * @brief A component contained in a Tab
 *
 */
class Animated {
 protected:
  Tab* tab;
  MainCanvas* c;

 public:
  Animated(Tab* tab);

  // Allow members to override init, render, and update
  // Tabs iterate through each of these manually

  virtual void init();
  virtual void render();
  virtual void update();

  Tab* getParentTab();
  MainCanvas* getParentCanvas();
};

#include "Member.hh"

#include "opengl/GrailGUI.hh"

using namespace std;

// BUG: If Frame Rate exceeds refresh rate (noticed at 120fps where
// default is 60fps), animation breaks at the Tab level
Member::Member(Tab* tab, double frameRate, double dt)
    : tab(tab), c(tab->getMainCanvas()) {
  tab->setFrameRate(frameRate);
  tab->setDt(dt);
  tab->setDefaultDt(0.0001);
  tab->addMember(this);
}

void Member::setFrameRate(double frameRate) { tab->setFrameRate(frameRate); }

void Member::setModelDt(double dt, double defaultDt) {
  tab->setDt(dt);
  tab->setDefaultDt(defaultDt);
}

void Member::init() {}
void Member::render() {}
void Member::update() {}

Tab* Member::getParentTab() { return tab; }
MainCanvas* Member::getParentCanvas() { return c; }
Member::Member(const Member& orig) = default;
Member::Member(Member&& orig) noexcept : tab(orig.tab), c(orig.c) {
  orig.tab = nullptr;
  orig.c = nullptr;
}
Member& Member::operator=(Member orig) {
  swap(*this, orig);
  return *this;
}
Member& Member::operator=(Member&& orig) noexcept {
  if (this != &orig) {
    tab = orig.tab;
    c = orig.c;
  }
  return *this;
}

void swap(Member& a, Member& b) {
  using std::swap;
  swap(a.tab, b.tab);
  swap(a.c, b.c);
}

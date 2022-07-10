#include "opengl/ChessBoardUtil.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"
#include "opengl/util/TextureArray.hh"

using namespace std;
using namespace grail;

class TestChessBoard : public Member {
 private:
  ChessBoard* chess;

 public:
  TestChessBoard(Tab* tab, GLWin* w) : Member(tab, 0, .1) {
    chess = new ChessBoard(c, w, tab, 100, 100, 600, 600);
    tab->bindEvent(Tab::MOUSE0_PRESS, [w, this]() { chess->press(w); });
  }

  void update() { chess->update(); }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test filldraw");
  MainCanvas* c = defaultTab->getMainCanvas();
  new TestChessBoard(defaultTab, w);
}
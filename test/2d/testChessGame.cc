#include "opengl/GrailGUI.hh"
#include "opengl/games/ChessGame.cc"

using namespace std;
using namespace grail;

class testChessGame : public Member {
 private:
  ChessGame* chess;

 public:
  testChessGame(Tab* tab, GLWin* w) : Member(tab) {
    chess = new ChessGame(tab, w);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test filldraw");
  MainCanvas* c = defaultTab->getMainCanvas();
  new testChessGame(defaultTab, w);
}
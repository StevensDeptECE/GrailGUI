#include "opengl/ChessBoardUtil.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"
#include "opengl/util/TextureArray.hh"

using namespace std;
using namespace grail;

class TestChessBoard : public Member {
 public:
  TestChessBoard(Tab* tab) : Member(tab, -1, 0) {
    ChessBoard(c, tab, 100, 100, 600, 600);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test filldraw");
  MainCanvas* c = defaultTab->getMainCanvas();
  new TestChessBoard(defaultTab);
}
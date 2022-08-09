#include "opengl/GrailGUI.hh"
#if 0
#include "opengl/games/ChessBoard.hh"
#include "opengl/games/ChessController.hh"
#include "opengl/games/ChessVisual.hh"
#endif

#include "opengl/games/ChessGame.hh"

using namespace std;
using namespace grail;

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test filldraw");
  MainCanvas* c = defaultTab->getMainCanvas();
  new ChessGame(defaultTab, w, c);
}
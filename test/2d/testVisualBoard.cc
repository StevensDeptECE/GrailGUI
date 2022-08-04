#include "opengl/GrailGUI.hh"
#include "opengl/games/ChessBoard.cc"
#include "opengl/games/ChessController.cc"
#include "opengl/games/ChessVisual.cc"

using namespace std;
using namespace grail;

class TestChessBoard : public Member {
 private:
  ChessController* controller;
  ChessVisual* visualMethods;
  ChessBoard* model;

 public:
  TestChessBoard(Tab* tab, GLWin* w, MainCanvas* c) : Member(tab, 0, 0.1) {
    model = new ChessBoard();
    model->move("a1a3");
    model->save("Chess.txt");
    model->load("Chess.txt");
    controller = new ChessController(model);
    visualMethods = new ChessVisual(controller, c, w, tab, 100, 100, 600, 600);
  }

  void update() { visualMethods->update(); }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test filldraw");
  MainCanvas* c = defaultTab->getMainCanvas();
  new TestChessBoard(defaultTab, w, c);
}
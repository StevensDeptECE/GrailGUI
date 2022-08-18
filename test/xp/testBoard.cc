//#include "opengl/games/Board.hh"
#include "opengl/games/ChessBoard.cc"
//#include "opengl/games/CheckerBoard.hh"
using namespace std;

void testChessBoard() {
  ChessBoard b;          // Create a chessboard 8x8
  b.save("test.Chess");  // TODO: lookup standard chess format
                         // b.move("b2b4");
  //  b.move('b', 2, 'b', 4);
  b.move(1, 6, 1, 4);
  b.remove(0, 0);
  cout << b << "\n";
  b.load("test.Chess");
  cout << b << "\n";
}

#if 0
void testCheckerBoard(){
  CheckerBoard b;
  b.save("test.Checkers");
  b.move("c2d3");
  cout << b;
  b.load("test.Checkers");
  cout << b;
}
#endif

int main() {
  testChessBoard();
  // testCheckerBoard();
}

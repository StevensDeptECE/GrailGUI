#pragma once

#include "opengl/GrailGUI.hh"
#include "opengl/games/ChessBoard.cc"
#include "opengl/games/ChessVisual.cc"

// Chess Controller is not useful, just copies from chessboard
class ChessGame : public Member {
 private:
  ChessVisual* visualMethods;
  ChessBoard* chess_pieces;

 public:
  ChessGame(Tab* tab, GLWin* w, MainCanvas* c) : Member(tab, 0, 0.1) {
    chess_pieces = new ChessBoard();
    visualMethods =
        new ChessVisual(chess_pieces, c, w, tab, 100, 100, 600, 600);
  };
  void update() { visualMethods->update(); };
};
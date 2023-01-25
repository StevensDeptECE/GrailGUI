#pragma once

#include "opengl/GrailGUI.hh"
#include "opengl/games/ChessBoard.cc"
#include "opengl/games/ChessServer.cc"
#include "opengl/games/ChessVisual.cc"

// Chess Controller is not useful, just copies from chessboard
class ChessGame : public Member {
 private:
  ChessVisual* visualMethods;
  ChessBoard* chess_pieces;
  ChessServer* server;

 public:
  ChessGame(Tab* tab, GLWin* w, MainCanvas* c, bool a) : Member(tab, 0, 0.1) {
    chess_pieces = new ChessBoard();
    server = new ChessServer(L"127.0.0.1", a);
    visualMethods =
        new ChessVisual(server, chess_pieces, c, w, tab, 100, 100, 600, 600);
  };
  void update() { visualMethods->update(); };
};
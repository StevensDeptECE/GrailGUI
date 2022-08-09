#include "opengl/games/Chess/ChessClasses.hh"

History::History(SquareVariables f, SquareVariables t, Board* b, History* p) {
  from = f;
  to = t;
  board = b;
  prev = p;
  next = NULL;
  killed_piece = board->PieceAt(to);
}

History::setNext(History* n) {}
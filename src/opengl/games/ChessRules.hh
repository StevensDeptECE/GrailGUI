#include "opengl/games/ChessBoard.hh"

class ChessBoard;

class ChessRules {
 private:
  struct move {
    int8_t fromRow;
    int8_t fromColumn;
    int8_t toRow;
    int8_t toColumn;
    int8_t piece;
    int8_t color;
  };
  ChessBoard* chess_pieces;

  //=========COOL FUN DATA WOWOWO=======//
 public:
  // std::vector <ChessRules>
  void PossibleMoves(ChessBoard* chess_pieces);
};
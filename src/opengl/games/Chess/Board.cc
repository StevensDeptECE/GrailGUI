#include "opengl/games/Chess/ChessClasses.hh"

Board::Board() {
  Piece *temp_piece;

  turn.setTurn('w');
  first_history = last_history = NULL;
  // en_passant = NULL;
  for (int i; i < width; i++)
    for (int j; j < width; j++) {
      temp_piece =  // the correct piece (NULL for empty squares)
          the_board[i][j] = temp_piece;
    }
};

// Method moves the piece occupying p1 to p2
// without checking wheter the move is legal
void Board::move(SquareVariables p1, SquareVariables p2, SquareVariables ep){
    /*
    add the move from p1 to p2 to the history;
    en_passant = ep;
    change the turn;
    set location p2 on the board to point to the piece that is at p1 now;
    set location p1 to NULL;
    */
};

void Board::move(SquareVariables p1, SquareVariables p2) {
  move(p1, p2, NULL);
};

#pragma once
#include "opengl/Image.hh"
#include "opengl/games/ChessBoard.hh"

class ChessBoard;

class Piece {
 protected:
  ChessBoard* board;
  uint64_t bitboard = 0;

 public:
  Piece(ChessBoard* board) : board(board){};
  virtual uint64_t checkPossibleMoves(uint8_t row, uint8_t column,
                                      uint8_t color) {
    bitboard = ~0;
    return bitboard;
  };
  bool checkLegalMove(uint64_t bitboard, uint8_t row, uint8_t column);
  uint64_t changeBitBoard(uint64_t bitboard, uint8_t row, uint8_t column);
};

class Knight : public Piece {
 public:
  Knight(ChessBoard* board) : Piece(board){};
  uint64_t checkPossibleMoves(uint8_t row, uint8_t column, uint8_t color){};
};

class Pawn : public Piece {
 public:
  Pawn(ChessBoard* board) : Piece(board){};
  uint64_t checkPossibleMoves(uint8_t row, uint8_t column, uint8_t color){};
};

class Rook : public Piece {
 public:
  Rook(ChessBoard* board) : Piece(board){};
  uint64_t checkPossibleMoves(uint8_t row, uint8_t column,
                              uint8_t color) override;
};

class Bishop : public Piece {
 public:
  Bishop(ChessBoard* board) : Piece(board){};
  uint64_t checkPossibleMoves(uint8_t row, uint8_t column, uint8_t color){};
};

class King : public Piece {
 public:
  King(ChessBoard* board) : Piece(board){};
  uint64_t checkPossibleMoves(uint8_t row, uint8_t column, uint8_t color){};
};

class Queen : public Piece {
 public:
  Queen(ChessBoard* board) : Piece(board){};
  uint64_t checkPossibleMoves(uint8_t row, uint8_t column, uint8_t color){};
};
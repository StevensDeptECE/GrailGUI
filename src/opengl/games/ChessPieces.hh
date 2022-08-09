#pragma once
#include "opengl/Image.hh"
#include "opengl/games/ChessBoard.hh"

class Piece {
 private:
  int8_t piece;

 public:
};

class Knight : Piece {};

class Pawn : Piece {};

class Rook : Piece {};

class Bishop : Piece {};

class King : Piece {};

class Queen : Piece {};
#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

extern const char PieceDisplayw[] = " RNBQKP";
extern const char PieceDisplayb[] = " rnbqkp";

enum class Color { black = 0, white = 1 };

enum class ChessPieceType {
  empty = 0,
  rook = 1,
  knight = 2,
  bishop = 3,
  queen = 4,
  king = 5,
  pawn = 6
};

class BoardPosition {
 private:
  friend class ChessBoard;
  friend class ostream;
#if 0
  uint8_t color : 1;
  uint8_t piece : 7;
#endif

 public:
#if 1
  uint8_t color : 1;  // TODO: ostream access issue
  uint8_t piece : 7;
#endif

  BoardPosition() {
    color = 0;
    piece = 0;
  }
  BoardPosition(Color color, ChessPieceType piece)
      : color(uint8_t(color)), piece(uint8_t(piece)){};
};
/**
 * @brief Internal representation utilizing FEN specification
 *
 */
class ChessBoard {
 private:
  char turn = 'w';
  BoardPosition board_position[8][8];

 public:
  ChessBoard();
  void save(const char filename[]);
  void load(const char filename[]);
  void move(const char moveLocation[]);
  void move(char fromRow, int8_t fromColumn, char toRow, int8_t toColumn);
  void move(uint16_t from, uint16_t to);
  friend std::ostream& operator<<(std::ostream& s, ChessBoard& b);
};

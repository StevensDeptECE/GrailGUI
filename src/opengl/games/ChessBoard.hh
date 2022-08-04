#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include "opengl/games/ChessController.hh"
extern const char PieceDisplayw[] = " RNBQKP";
extern const char PieceDisplayb[] = " rnbqkp";

class ChessVisual;
class ChessController;

enum class ChessColor { black = 0, white = 1 };

enum class ChessPieceType : uint8_t {
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
  friend class ChessController;
#if 0
  uint8_t color : 1;
  uint8_t piece : 7;
#endif

 public:
#if 1
  uint8_t color : 2;  // TODO: ostream access issue
  uint8_t piece : 7;
#endif

  BoardPosition() {
    color = 0;
    piece = 0;
  }
  BoardPosition(ChessColor color, ChessPieceType piece)
      : color(uint8_t(color)), piece(uint8_t(piece)){};
};
/**
 * @brief Internal representation utilizing FEN specification
 *
 */
class ChessBoard {
 private:
  char turn;
  friend ChessVisual;
  friend ChessController;
  BoardPosition board_position[8][8];

 public:
  ChessBoard();
  void save(const char filename[]);
  void load(const char filename[]);
  void move(const char moveLocation[]);
  // Possible best method internally?:
  // void move(char fromColumn, int8_t fromRow, char toColumn, int8_t toRow);
  void move(int8_t oldColumn, int8_t oldRow, int8_t newColumn, int8_t newRow);
  void remove(const char pieceLocation[]);
  void remove(char Column, int8_t Row);

  friend std::ostream& operator<<(std::ostream& s, ChessBoard& b);

  // util
#if 0
  std::vector<vector<BoardPosition>> getBoardPosition(ChessBoard& b);

  template <typename T, int N>
  struct vector_wrapper {
    vector_wrapper(T (&a)[N]) { std::copy(a, a + N, std::back_inserter(v)); }

    std::vector<T> v;
  };

  std::vector<vector_wrapper<BoardPosition, 8>> getBoardPosition();
#endif
};

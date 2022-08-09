#pragma once
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

extern const char PieceDisplayw[] = " RNBQKP";
extern const char PieceDisplayb[] = " rnbqkp";

class ChessVisual;
class ChessController;
class ChessBoard;
class ChessRules;

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
  friend std::ostream& operator<<(std::ostream& s, ChessBoard& b);

  uint8_t color : 1;
  uint8_t piece : 7;

 public:
  BoardPosition() {
    color = 0;
    piece = 0;
  }
  BoardPosition(ChessColor color, ChessPieceType piece)
      : color(uint8_t(color)), piece(uint8_t(piece)){};
};

class Observer {
 public:
  void update();
};
class Data {
 private:
  std::vector<Observer*> observers;

 public:
  void changed() {
    for (auto obs : observers) obs->update();
  }
  void addObserver(Observer* obs) { observers.push_back(obs); }
};

/**
 * @brief Internal representation utilizing FEN specification
 *
 */
class ChessBoard : public Data {
 private:
  char turn;
  friend ChessVisual;
  friend ChessRules;
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
  int8_t getPiece(int8_t row, int8_t column);
  int8_t getColor(int8_t row, int8_t column);
  int8_t getTurn();
  bool checkTurn(int8_t row, int8_t column);
  friend std::ostream& operator<<(std::ostream& s, ChessBoard& b);
};

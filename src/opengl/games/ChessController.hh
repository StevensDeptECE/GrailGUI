#pragma once

#include "opengl/GrailGUI.hh"
#include "opengl/games/ChessBoard.hh"

class ChessVisual;
class ChessBoard;

class ChessController {
 private:
  ChessBoard* model;

 public:
  ChessController(ChessBoard* model);
  bool CheckMove(/*to, from*/);
  void setMove(int8_t oldColumn, int8_t oldRow, int8_t newColumn,
               int8_t newRow);
  int8_t getPiece(int8_t row, int8_t column);
  int8_t getColor(int8_t row, int8_t column);
  bool checkTurn(int8_t row, int8_t column);
  int8_t getTurn();
  void getModel();
};
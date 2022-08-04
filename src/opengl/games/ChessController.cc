#include "opengl/games/ChessController.hh"

ChessController::ChessController(ChessBoard* model) : model(model) {}

void ChessController::setMove(int8_t oldColumn, int8_t oldRow, int8_t newColumn,
                              int8_t newRow) {
  model->move(oldColumn, oldRow, newColumn, newRow);
}

int8_t ChessController::getColor(int8_t row, int8_t column) {
  return model->board_position[row][column].color;
}

int8_t ChessController::getPiece(int8_t row, int8_t column) {
  return model->board_position[row][column].piece;
}

int8_t ChessController::getTurn() {
  if (model->turn == 'w')
    return 1;
  else
    return 0;
}

bool ChessController::checkTurn(int8_t row, int8_t column) {
  if (getTurn() == model->board_position[row][column].color) {
    return true;
  } else {
    return false;
  }
}
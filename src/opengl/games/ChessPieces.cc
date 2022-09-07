#include "opengl/games/ChessPieces.hh"
using namespace std;

bool Piece::checkLegalMove(uint64_t bitboard, uint8_t row, uint8_t column) {
  uint64_t place = 1;
  uint64_t spot = row * 8 + column;
  place = place << spot;
  place = bitboard & place;
  if (place != 0) {
    return true;
  } else {
    return false;
  }
}

uint64_t Piece::changeBitBoard(uint64_t bitboard, uint8_t row, uint8_t column) {
  uint64_t place = 1;
  uint64_t spot = row * 8 + column;
  place = place << spot;
  place = bitboard | place;
  return place;
}

uint64_t Rook::checkPossibleMoves(uint8_t row, uint8_t column, uint8_t color) {
  bitboard = 0;
  for (uint8_t i = row + 1; i < 8; i++) {
    if (board->getPiece(i, column) != 0 &&
        board->getColor(i, column) == color) {
      break;
    } else if (board->getPiece(i, column) != 0 &&
               board->getColor(i, column) != color) {
      bitboard = changeBitBoard(bitboard, i, column);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, column);
    }
  }

  // up
  for (uint8_t i = row - 1; i >= 0; i--) {
    if (board->getPiece(i, column) != 0 &&
        board->getColor(i, column) == color) {
      break;
    } else if (board->getPiece(i, column) != 0 &&
               board->getColor(i, column) != color) {
      bitboard = changeBitBoard(bitboard, i, column);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, column);
    }
  }

  // left
  for (uint8_t j = column - 1; j >= 0; j--) {
    if (board->getPiece(row, j) != 0 && board->getColor(row, j) == color) {
      break;
    } else if (board->getPiece(row, j) != 0 &&
               board->getColor(row, j) != color) {
      bitboard = changeBitBoard(bitboard, row, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, row, j);
    }
  }

  // right
  for (uint8_t j = column + 1; j < 8; j++) {
    if (board->getPiece(row, j) != 0 && board->getColor(row, j) == color) {
      break;
    } else if (board->getPiece(row, j) != 0 &&
               board->getColor(row, j) != color) {
      bitboard = changeBitBoard(bitboard, row, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, row, j);
    }
  }
  return bitboard;
}
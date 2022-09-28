#include "opengl/games/ChessBoard.hh"
using namespace std;

ChessBoard::ChessBoard() {
  for (int i = 0; i < 8; i++) {
    int piecekind = 0;
    int piececounter = 0;
    for (int j = 0; j < 8; j++) {
      if (i == 0 || i == 7) {
        if (i == 0) board_position[i][j].color = 0;
        if (i == 7) board_position[i][j].color = 1;
        if (piecekind < 5 && piececounter < 5) {
          piecekind++;
          piececounter++;
        } else {
          if (piecekind == 5) piecekind = 4;
          piecekind--;
        }
        board_position[i][j].piece = piecekind;
        switch (piecekind) {
          case 1:
            board_position[i][j].chesspiece = new Rook(this);
            break;
          case 2:
            board_position[i][j].chesspiece = new Knight(this);
            break;
          case 3:
            board_position[i][j].chesspiece = new Bishop(this);
            break;
          case 4:
            board_position[i][j].chesspiece = new Queen(this);
            break;
          case 5:
            board_position[i][j].chesspiece = new King(this);
            break;
        }
      }
      if (i == 1 || i == 6) {
        if (i == 1) board_position[i][j].color = 0;
        if (i == 6) board_position[i][j].color = 1;
        piecekind = 6;
        board_position[i][j].piece = piecekind;
        board_position[i][j].chesspiece = new Pawn(this);
      }
    }
  }

  turn = 'w';
}

int8_t ChessBoard::getColor(int8_t row, int8_t column) {
  return board_position[row][column].color;
}

int8_t ChessBoard::getPiece(int8_t row, int8_t column) {
  return board_position[row][column].piece;
}

int8_t ChessBoard::getTurn() {
  if (turn == 'w')
    return 1;
  else
    return 0;
}

bool ChessBoard::checkTurn(int8_t row, int8_t column) {
  if (getTurn() == board_position[row][column].color) {
    return true;
  } else {
    return false;
  }
}

// Saves games using
void ChessBoard::save(const char filename[]) {
  // TODO: castling availability, and enpassant targets
  ofstream saveFile(filename, ios::out | ofstream::trunc);

  for (int i = 0; i < 8; i++) {
    int emptySquareCount = 0;
    for (int j = 0; j < 8; j++) {
      if (board_position[i][j].piece == 0) {
        emptySquareCount++;
        if (board_position[i][j + 1].piece != 0 || j == 7) {
          saveFile << emptySquareCount;
          emptySquareCount = 0;
        }
      } else if (board_position[i][j].piece == 1 &&
                 board_position[i][j].color == 0)
        saveFile << "r";
      else if (board_position[i][j].piece == 2 &&
               board_position[i][j].color == 0)
        saveFile << "n";
      else if (board_position[i][j].piece == 3 &&
               board_position[i][j].color == 0)
        saveFile << "b";
      else if (board_position[i][j].piece == 4 &&
               board_position[i][j].color == 0)
        saveFile << "q";
      else if (board_position[i][j].piece == 5 &&
               board_position[i][j].color == 0)
        saveFile << "k";
      else if (board_position[i][j].piece == 6 &&
               board_position[i][j].color == 0)
        saveFile << "p";
      else if (board_position[i][j].piece == 1 &&
               board_position[i][j].color == 1)
        saveFile << "R";
      else if (board_position[i][j].piece == 2 &&
               board_position[i][j].color == 1)
        saveFile << "N";
      else if (board_position[i][j].piece == 3 &&
               board_position[i][j].color == 1)
        saveFile << "B";
      else if (board_position[i][j].piece == 4 &&
               board_position[i][j].color == 1)
        saveFile << "Q";
      else if (board_position[i][j].piece == 5 &&
               board_position[i][j].color == 1)
        saveFile << "K";
      else if (board_position[i][j].piece == 6 &&
               board_position[i][j].color == 1)
        saveFile << "P";
    }
    saveFile << "/";
  }

  saveFile << " " << turn;
  saveFile.close();
}

void ChessBoard::load(const char filename[]) {
  // TODO: Current load supports only FEN notation, perhaps add PGN in the
  // future
  // TODO: After creating rules, load castling and en passant tags
  string ChessPosition;
  ifstream loadFile(filename);
  getline(loadFile, ChessPosition);

  vector<int32_t> rowList;

  for (auto ch : ChessPosition) {
    rowList.push_back(ch);
  }

  /*for (int i = 0; i     rowList.size(); i++) {
    cout <    rowList[i] << " ";
  }*/

  int rowCounter = 0;
  int columnCounter = 0;

  for (int i = 0; i < rowList.size(); i++) {
    // 32 = SPACE
    if (rowList[i] == 32) {
      // 98 = b
      if (rowList[i + 1] == 98) {
        turn = 'b';
        break;
      } else {
        turn = 'w';
        break;
      }
    }
    // 47 = /
    else if (rowList[i] == 47) {
      columnCounter++;
      rowCounter = 0;
    }
    // 49 = 1
    else if (rowList[i] == 49) {
      board_position[columnCounter][rowCounter].piece = 0;
      rowCounter++;
    }
    // 50 = 2
    else if (rowList[i] == 50) {
      for (int i = 0; i < 2; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 51 = 3
    else if (rowList[i] == 51) {
      for (int i = 0; i < 3; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 52 = 4
    else if (rowList[i] == 52) {
      for (int i = 0; i < 4; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 53 = 5
    else if (rowList[i] == 53) {
      for (int i = 0; i < 5; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 54 = 6
    else if (rowList[i] == 54) {
      for (int i = 0; i < 6; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 55 = 7
    else if (rowList[i] == 55) {
      for (int i = 0; i < 7; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 56 = 8
    else if (rowList[i] == 56) {
      for (int i = 0; i < 8; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 66 = B
    else if (rowList[i] == 66) {
      board_position[columnCounter][rowCounter].piece = 3;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 75 = K
    else if (rowList[i] == 75) {
      board_position[columnCounter][rowCounter].piece = 5;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 79 = N
    else if (rowList[i] == 78) {
      board_position[columnCounter][rowCounter].piece = 2;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 80 = P
    else if (rowList[i] == 80) {
      board_position[columnCounter][rowCounter].piece = 6;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 81 = Q
    else if (rowList[i] == 81) {
      board_position[columnCounter][rowCounter].piece = 4;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 82 = R
    else if (rowList[i] == 82) {
      board_position[columnCounter][rowCounter].piece = 1;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 98 = b
    else if (rowList[i] == 98) {
      board_position[columnCounter][rowCounter].piece = 3;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 107 = k
    else if (rowList[i] == 107) {
      board_position[columnCounter][rowCounter].piece = 5;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 110 = n
    else if (rowList[i] == 110) {
      board_position[columnCounter][rowCounter].piece = 2;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 112 = p
    else if (rowList[i] == 112) {
      board_position[columnCounter][rowCounter].piece = 6;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 113 = q
    else if (rowList[i] == 113) {
      board_position[columnCounter][rowCounter].piece = 4;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 114 = r
    else if (rowList[i] == 114) {
      board_position[columnCounter][rowCounter].piece = 1;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
  }
}

void ChessBoard::move(const char moveLocation[]) {
  // TODO:IGNORES FIDE NOTATION, TAKES FIRST TWO CHARACTERS AS THE FROM AND THE
  // NEXT TWO AS THE TO
  vector<int8_t> rowList;
  vector<int8_t> columnList;

  // Note: The first row on a chessboard labeled by the number one
  // corresponds to the seventh row of the matrix in board_position
  for (int i = 0; moveLocation[i] != 0; i++) {
    // 49 = 1
    if (moveLocation[i] == 49) {
      rowList.push_back(7);
    }
    // 50 = 2
    else if (moveLocation[i] == 50) {
      rowList.push_back(6);
    }
    // 51 = 3
    else if (moveLocation[i] == 51) {
      rowList.push_back(5);
    }
    // 52 = 4
    else if (moveLocation[i] == 52) {
      rowList.push_back(4);
    }
    // 53 = 5
    else if (moveLocation[i] == 53) {
      rowList.push_back(3);
    }
    // 54 = 6
    else if (moveLocation[i] == 54) {
      rowList.push_back(2);
    }
    // 55 = 7
    else if (moveLocation[i] == 55) {
      rowList.push_back(1);
    }
    // 56 = 8
    else if (moveLocation[i] == 56) {
      rowList.push_back(0);
    }
    // a = 97 = first column
    else if (moveLocation[i] == 97) {
      columnList.push_back(0);
    }
    // b = 98 = second column
    else if (moveLocation[i] == 98) {
      columnList.push_back(1);
    }
    // c = 99 = third column
    else if (moveLocation[i] == 99) {
      columnList.push_back(2);
    }
    // d = 100 = fourth column
    else if (moveLocation[i] == 100) {
      columnList.push_back(3);
    }
    // e = 101 = fifth column
    else if (moveLocation[i] == 101) {
      columnList.push_back(4);
    }
    // f = 102 = sixth column
    else if (moveLocation[i] == 102) {
      columnList.push_back(5);
    }
    // g = 103 = seventh column
    else if (moveLocation[i] == 103) {
      columnList.push_back(6);
    }
    // h = 104 = eigth column
    else if (moveLocation[i] == 104) {
      columnList.push_back(7);
    }
  }

  board_position[rowList[1]][columnList[1]].piece =
      board_position[rowList[0]][columnList[0]].piece;
  board_position[rowList[1]][columnList[1]].color =
      board_position[rowList[0]][columnList[0]].color;
  board_position[rowList[0]][columnList[0]].piece = 0;

  if (turn == 'w')
    turn = 'b';
  else
    turn = 'w';
}

#if 0
void ChessBoard::move(char fromColumn, int8_t fromRow, char toColumn,
                      int8_t toRow) {
  char symbolArray[] = {fromColumn, toColumn, 0};
  vector<int8_t> columnList;

  for (int i = 0; symbolArray[i] != 0; i++) {
    // a = 97 = first column
    if (symbolArray[i] == 97) {
      columnList.push_back(0);
    }
    // b = 98 = second column
    else if (symbolArray[i] == 98) {
      columnList.push_back(1);
    }
    // c = 99 = third column
    else if (symbolArray[i] == 99) {
      columnList.push_back(2);
    }
    // d = 100 = fourth column
    else if (symbolArray[i] == 100) {
      columnList.push_back(3);
    }
    // e = 101 = fifth column
    else if (symbolArray[i] == 101) {
      columnList.push_back(4);
    }
    // f = 102 = sixth column
    else if (symbolArray[i] == 102) {
      columnList.push_back(5);
    }
    // g = 103 = seventh column
    else if (symbolArray[i] == 103) {
      columnList.push_back(6);
    }
    // h = 104 = eigth column
    else if (symbolArray[i] == 104) {
      columnList.push_back(7);
    }
  }

  // Equations to account for the flipping of the chessboard
  // and also the fact that the board is structured 1-8 not 0-7 like the array
  fromRow = fromRow - 8 + ((-1) * (2 * (fromRow - 8)));
  toRow = toRow - 8 + ((-1) * (2 * (toRow - 8)));

  board_position[toRow][columnList[1]].piece =
      board_position[fromRow][columnList[0]].piece;
  board_position[toRow][columnList[1]].color =
      board_position[fromRow][columnList[0]].color;
  board_position[fromRow][columnList[0]].piece = 0;

  if (turn == 'w')
    turn = 'b';
  else
    turn = 'w';
}
#endif

bool ChessBoard::move(uint8_t oldColumn, uint8_t oldRow, uint8_t newColumn,
                      uint8_t newRow) {
  // specify the actual positions within the array in parameters rather than in
  // chess notation
  // 2d mapped into 1d --> array[width * height] To find the
  // number in the array --> array[width * row + column]
  uint64_t bitboard =
      board_position[oldRow][oldColumn].chesspiece->checkPossibleMoves(
          oldRow, oldColumn, board_position[oldRow][oldColumn].color);
  bool possibleMove =
      board_position[oldRow][oldColumn].chesspiece->checkLegalMove(
          bitboard, newRow, newColumn);
  if (possibleMove) {
    board_position[newRow][newColumn].piece =
        board_position[oldRow][oldColumn].piece;
    board_position[newRow][newColumn].color =
        board_position[oldRow][oldColumn].color;
    board_position[oldRow][oldColumn].piece = 0;
    board_position[newRow][newColumn].chesspiece =
        board_position[oldRow][oldColumn].chesspiece;
    board_position[oldRow][oldColumn].chesspiece = nullptr;
    if (turn == 'w')
      turn = 'b';
    else
      turn = 'w';

    return true;
  } else {
    return false;
  }
}

void ChessBoard::remove(const char pieceLocation[]) {
  int8_t row;
  int8_t column;

  for (int i = 0; pieceLocation[i] != 0; i++) {
    // 49 = 1
    if (pieceLocation[i] == 49) {
      row = 7;
    }
    // 50 = 2
    else if (pieceLocation[i] == 50) {
      row = 6;
    }
    // 51 = 3
    else if (pieceLocation[i] == 51) {
      row = 5;
    }
    // 52 = 4
    else if (pieceLocation[i] == 52) {
      row = 4;
    }
    // 53 = 5
    else if (pieceLocation[i] == 53) {
      row = 3;
    }
    // 54 = 6
    else if (pieceLocation[i] == 54) {
      row = 2;
    }
    // 55 = 7
    else if (pieceLocation[i] == 55) {
      row = 1;
    }
    // 56 = 8
    else if (pieceLocation[i] == 56) {
      row = 0;
    }
    // a = 97 = first column
    else if (pieceLocation[i] == 97) {
      column = 0;
    }
    // b = 98 = second column
    else if (pieceLocation[i] == 98) {
      column = 1;
    }
    // c = 99 = third column
    else if (pieceLocation[i] == 99) {
      column = 2;
    }
    // d = 100 = fourth column
    else if (pieceLocation[i] == 100) {
      column = 3;
    }
    // e = 101 = fifth column
    else if (pieceLocation[i] == 101) {
      column = 4;
    }
    // f = 102 = sixth column
    else if (pieceLocation[i] == 102) {
      column = 5;
    }
    // g = 103 = seventh column
    else if (pieceLocation[i] == 103) {
      column = 6;
    }
    // h = 104 = eigth column
    else if (pieceLocation[i] == 104) {
      column = 7;
    }
  }

  board_position[row][column].piece = 0;
}

void ChessBoard::remove(char Column, int8_t Row) {
  int8_t column;

  // a = 97 = first column
  if (Column == 97) {
    column = 0;
  }
  // b = 98 = second column
  else if (Column == 98) {
    column = 1;
  }
  // c = 99 = third column
  else if (Column == 99) {
    column = 2;
  }
  // d = 100 = fourth column
  else if (Column == 100) {
    column = 3;
  }
  // e = 101 = fifth column
  else if (Column == 101) {
    column = 4;
  }
  // f = 102 = sixth column
  else if (Column == 102) {
    column = 5;
  }
  // g = 103 = seventh column
  else if (Column == 103) {
    column = 6;
  }
  // h = 104 = eigth column
  else if (Column == 104) {
    column = 7;
  }

  board_position[Row][column].piece = 0;
}

ostream& operator<<(ostream& s, ChessBoard& b) {
  for (uint32_t i = 0; i < 8; i++) {
    for (uint32_t j = 0; j < 8; j++) {
      if (b.board_position[i][j].color == 0)
        s << PieceDisplayb[b.board_position[i][j].piece];
      else
        s << PieceDisplayw[b.board_position[i][j].piece];
    }
    s << "\n";
  }
  return s;
}

uint64_t Piece::checkPossibleMoves(uint8_t row, uint8_t column, uint8_t color) {
  bitboard = ~0;
  return bitboard;
}

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
  for (int8_t i = row + 1; i < 8; i++) {
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
  for (int8_t i = row - 1; i >= 0; i--) {
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
  for (int8_t j = column - 1; j >= 0; j--) {
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
  for (int8_t j = column + 1; j < 8; j++) {
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

uint64_t Bishop::checkPossibleMoves(uint8_t row, uint8_t column,
                                    uint8_t color) {
  bitboard = 0;
  for (int8_t i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++) {
    if (board->getPiece(i, j) != 0 && board->getColor(i, j) == color) {
      break;
    } else if (board->getPiece(i, j) != 0 && board->getColor(i, j) != color) {
      bitboard = changeBitBoard(bitboard, i, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, j);
    }
  }
  for (int8_t i = row - 1, j = column + 1; i >= 0 && j < 8; i--, j++) {
    if (board->getPiece(i, j) != 0 && board->getColor(i, j) == color) {
      break;
    } else if (board->getPiece(i, j) != 0 && board->getColor(i, j) != color) {
      bitboard = changeBitBoard(bitboard, i, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, j);
    }
  }
  for (int8_t i = row + 1, j = column - 1; i < 8 && j >= 0; i++, j--) {
    if (board->getPiece(i, j) != 0 && board->getColor(i, j) == color) {
      break;
    } else if (board->getPiece(i, j) != 0 && board->getColor(i, j) != color) {
      bitboard = changeBitBoard(bitboard, i, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, j);
    }
  }
  for (int8_t i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
    if (board->getPiece(i, j) != 0 && board->getColor(i, j) == color) {
      break;
    } else if (board->getPiece(i, j) != 0 && board->getColor(i, j) != color) {
      bitboard = changeBitBoard(bitboard, i, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, j);
    }
  }
  return bitboard;
}

uint64_t Queen::checkPossibleMoves(uint8_t row, uint8_t column, uint8_t color) {
  bitboard = 0;
  for (int8_t i = row + 1; i < 8; i++) {
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
  for (int8_t i = row - 1; i >= 0; i--) {
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
  for (int8_t j = column - 1; j >= 0; j--) {
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
  for (int8_t j = column + 1; j < 8; j++) {
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
  for (int8_t i = row + 1, j = column + 1; i < 8 && j < 8; i++, j++) {
    if (board->getPiece(i, j) != 0 && board->getColor(i, j) == color) {
      break;
    } else if (board->getPiece(i, j) != 0 && board->getColor(i, j) != color) {
      bitboard = changeBitBoard(bitboard, i, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, j);
    }
  }
  for (int8_t i = row - 1, j = column + 1; i >= 0 && j < 8; i--, j++) {
    if (board->getPiece(i, j) != 0 && board->getColor(i, j) == color) {
      break;
    } else if (board->getPiece(i, j) != 0 && board->getColor(i, j) != color) {
      bitboard = changeBitBoard(bitboard, i, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, j);
    }
  }
  for (int8_t i = row + 1, j = column - 1; i < 8 && j >= 0; i++, j--) {
    if (board->getPiece(i, j) != 0 && board->getColor(i, j) == color) {
      break;
    } else if (board->getPiece(i, j) != 0 && board->getColor(i, j) != color) {
      bitboard = changeBitBoard(bitboard, i, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, j);
    }
  }
  for (int8_t i = row - 1, j = column - 1; i >= 0 && j >= 0; i--, j--) {
    if (board->getPiece(i, j) != 0 && board->getColor(i, j) == color) {
      break;
    } else if (board->getPiece(i, j) != 0 && board->getColor(i, j) != color) {
      bitboard = changeBitBoard(bitboard, i, j);
      break;
    } else {
      bitboard = changeBitBoard(bitboard, i, j);
    }
  }
  return bitboard;
}
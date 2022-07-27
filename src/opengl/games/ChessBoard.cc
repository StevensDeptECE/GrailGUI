#include "opengl/games/ChessBoard.hh"
using namespace std;

ChessBoard::ChessBoard() {
  // TODO: How do you represent black or white pieces?
  //  How to represent all the kinds of pieces
  //  How do you setup the whole board?
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
      }
      if (i == 1 || i == 6) {
        if (i == 1) board_position[i][j].color = 0;
        if (i == 6) board_position[i][j].color = 1;
        piecekind = 6;
        board_position[i][j].piece = piecekind;
      }
    }
  }
}

// Saves games using
void ChessBoard::save(const char filename[]) {
  // TODO: add active color, castling availability, and enpassant targets
  ofstream saveFile(filename);

  for (int i = 0; i < 8; i++) {
    int emptySquareCount = 0;
    for (int j = 0; j < 8; j++) {
      if (board_position[i][j].piece == 0) {
        emptySquareCount++;
        if (board_position[i][j + 1].piece != 0 || j == 7) {
          saveFile << emptySquareCount;
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
  vector<int32_t> rowList;
  vector<int32_t> columnList;

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
  board_position[rowList[0]][columnList[0]].color = 0;

  if (turn == 'w')
    turn = 'b';
  else
    turn = 'w';
}

/*void ChessBoard::move(char fromRow, int8_t fromColumn, char toRow,
                      int8_t toColumn) {}

void ChessBoard::move(uint16_t from, uint16_t to) {}*/

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

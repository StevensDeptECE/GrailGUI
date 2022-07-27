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

  vector<int32_t> symbolList;

  for (auto ch : ChessPosition) {
    symbolList.push_back(ch);
  }

  /*for (int i = 0; i < symbolList.size(); i++) {
    cout << symbolList[i] << " ";
  }*/

  int rowCounter = 0;
  int columnCounter = 0;

  for (int i = 0; i < symbolList.size(); i++) {
    // 32 = SPACE
    if (symbolList[i] == 32) {
      // 98 = b
      if (symbolList[i + 1] == 98) {
        turn = 'b';
        break;
      } else {
        turn = 'w';
        break;
      }
    }
    // 47 = /
    else if (symbolList[i] == 47) {
      columnCounter++;
      rowCounter = 0;
    }
    // 49 = 1
    else if (symbolList[i] == 49) {
      board_position[columnCounter][rowCounter].piece = 0;
      rowCounter++;
    }
    // 50 = 2
    else if (symbolList[i] == 50) {
      for (int i = 0; i < 2; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 51 = 3
    else if (symbolList[i] == 51) {
      for (int i = 0; i < 3; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 52 = 4
    else if (symbolList[i] == 52) {
      for (int i = 0; i < 4; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 53 = 5
    else if (symbolList[i] == 53) {
      for (int i = 0; i < 5; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 54 = 6
    else if (symbolList[i] == 54) {
      for (int i = 0; i < 6; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 55 = 7
    else if (symbolList[i] == 55) {
      for (int i = 0; i < 7; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 56 = 8
    else if (symbolList[i] == 56) {
      for (int i = 0; i < 8; i++) {
        board_position[columnCounter][rowCounter].piece = 0;
        rowCounter++;
      }
    }
    // 66 = B
    else if (symbolList[i] == 66) {
      board_position[columnCounter][rowCounter].piece = 3;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 75 = K
    else if (symbolList[i] == 75) {
      board_position[columnCounter][rowCounter].piece = 5;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 79 = N
    else if (symbolList[i] == 78) {
      board_position[columnCounter][rowCounter].piece = 2;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 80 = P
    else if (symbolList[i] == 80) {
      board_position[columnCounter][rowCounter].piece = 6;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 81 = Q
    else if (symbolList[i] == 81) {
      board_position[columnCounter][rowCounter].piece = 4;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 82 = R
    else if (symbolList[i] == 82) {
      board_position[columnCounter][rowCounter].piece = 1;
      board_position[columnCounter][rowCounter].color = 1;
      rowCounter++;
    }
    // 98 = b
    else if (symbolList[i] == 98) {
      board_position[columnCounter][rowCounter].piece = 3;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 107 = k
    else if (symbolList[i] == 107) {
      board_position[columnCounter][rowCounter].piece = 5;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 110 = n
    else if (symbolList[i] == 110) {
      board_position[columnCounter][rowCounter].piece = 2;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 112 = p
    else if (symbolList[i] == 112) {
      board_position[columnCounter][rowCounter].piece = 6;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 113 = q
    else if (symbolList[i] == 113) {
      board_position[columnCounter][rowCounter].piece = 4;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
    // 114 = r
    else if (symbolList[i] == 114) {
      board_position[columnCounter][rowCounter].piece = 1;
      board_position[columnCounter][rowCounter].color = 0;
      rowCounter++;
    }
  }
}

void ChessBoard::move(const char moveLocation[]) {}

void ChessBoard::move(char fromRow, int8_t fromColumn, char toRow,
                      int8_t toColumn) {}

void ChessBoard::move(uint16_t from, uint16_t to) {}

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

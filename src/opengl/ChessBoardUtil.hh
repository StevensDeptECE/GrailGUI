#include <stdlib.h>

#include <regex>

#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"
#include "opengl/MultiText.hh"

using namespace std;
using namespace grail;
/*
⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠛⠛⠋⠉⠈⠉⠉⠉⠉⠛⠻⢿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⡿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⢿⣿⣿⣿⣿
⣿⣿⣿⣿⡏⣀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿
⣿⣿⣿⢏⣴⣿⣷⠀⠀⠀⠀⠀⢾⣿⣿⣿⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿
⣿⣿⣟⣾⣿⡟⠁⠀⠀⠀⠀⠀⢀⣾⣿⣿⣿⣿⣿⣷⢢⠀⠀⠀⠀⠀⠀⠀⢸⣿
⣿⣿⣿⣿⣟⠀⡴⠄⠀⠀⠀⠀⠀⠀⠙⠻⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⣿
⣿⣿⣿⠟⠻⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠶⢴⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⣿
⣿⣁⡀⠀⠀⢰⢠⣦⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⡄⠀⣴⣶⣿⡄⣿
⣿⡋⠀⠀⠀⠎⢸⣿⡆⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣿⣿⣿⣿⠗⢘⣿⣟⠛⠿⣼
⣿⣿⠋⢀⡌⢰⣿⡿⢿⡀⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⡇⠀⢸⣿⣿⣧⢀⣼
⣿⣿⣷⢻⠄⠘⠛⠋⠛⠃⠀⠀⠀⠀⠀⢿⣧⠈⠉⠙⠛⠋⠀⠀⠀⣿⣿⣿⣿⣿
⣿⣿⣧⠀⠈⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠟⠀⠀⠀⠀⢀⢃⠀⠀⢸⣿⣿⣿⣿
⣿⣿⡿⠀⠴⢗⣠⣤⣴⡶⠶⠖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡸⠀⣿⣿⣿⣿
⣿⣿⣿⡀⢠⣾⣿⠏⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠛⠉⠀⣿⣿⣿⣿
⣿⣿⣿⣧⠈⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣰⣿⣿⣿⣿
⣿⣿⣿⣿⡄⠈⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣾⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣦⣄⣀⣀⣀⣀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠙⣿⣿⡟⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠁⠀⠀⠹⣿⠃⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⢐⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⠿⠛⠉⠉⠁⠀⢻⣿⡇⠀⠀⠀⠀⠀⠀⢀⠈⣿⣿⡿⠉⠛⠛⠛⠉⠉
⣿⡿⠋⠁⠀⠀⢀⣀⣠⡴⣸⣿⣇⡄⠀⠀⠀⠀⢀⡿⠄⠙⠛⠀⣀⣠⣤⣤⠄
*/
class ChessBoard {
 private:
  struct squareLocation {
    float xposition = 0;
    float yposition;
    float xleft;
    float ytop;
    float xright;
    float ybot;
    Image* currentPiece = nullptr;
    string filepath;
    string typeOfPiece;
    char color;
  };
  struct Square {
    int row;
    int column;
    bool takingPiece = false;
    Square(int row, int column) : row(row), column(column) {}
  };

  float xpos;
  float ypos;
  squareLocation ImageList[8][8];
  squareLocation* PreviousSquare[1][1];
  GLWin* window;
  Image* selectedPiece = nullptr;
  Image* clickPiece = nullptr;
  bool hasclicked = false;
  bool clickmove = false;
  float width;
  float height;
  float xstart;
  float ystart;
  MainCanvas* c;
  float sizeSquares;
  int selectedRow;
  int selectedColumn;
  int previousRow;
  int previousColumn;
  string selectedPath;
  string selectedTypeOfPiece;
  string selectedPathClick;
  char selectedColor;
  char turn = 'w';
  std::vector<Square> possibleSquares;

 public:
  ChessBoard(MainCanvas* c, GLWin* window, Tab* tab, float xstart, float ystart,
             float w, float h)
      : window(window),
        width(w),
        height(h),
        xstart(xstart),
        ystart(ystart),
        c(c) {
    sizeSquares = w / 8;
    calculateSquares(xstart, ystart, sizeSquares);

    //=============================Chess Board Initialization=====///
    StyledMultiShape2D* board =
        c->addLayer(new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0));
    board->fillDrawRectangle(xstart * 0.55, ystart * 0.55, w * 1.15, h * 1.15,
                             gray, black);
    board->CheckeredGrid(xstart, ystart, w, h, 8, 8, black, darkgreen,
                         lightgrey);

    // store filepaths//
    ImageList[0][0].filepath = "ChessTextures/brook.webp";
    ImageList[0][1].filepath = "ChessTextures/bknight.webp";
    ImageList[0][2].filepath = "ChessTextures/bbishop.webp";
    ImageList[0][3].filepath = "ChessTextures/bqueen.webp";
    ImageList[0][4].filepath = "ChessTextures/bking.webp";
    ImageList[0][5].filepath = "ChessTextures/bbishop.webp";
    ImageList[0][6].filepath = "ChessTextures/bknight.webp";
    ImageList[0][7].filepath = "ChessTextures/brook.webp";

    ImageList[7][0].filepath = "ChessTextures/wrook.webp";
    ImageList[7][1].filepath = "ChessTextures/wknight.webp";
    ImageList[7][2].filepath = "ChessTextures/wbishop.webp";
    ImageList[7][3].filepath = "ChessTextures/wqueen.webp";
    ImageList[7][4].filepath = "ChessTextures/wking.webp";
    ImageList[7][5].filepath = "ChessTextures/wbishop.webp";
    ImageList[7][6].filepath = "ChessTextures/wknight.webp";
    ImageList[7][7].filepath = "ChessTextures/wrook.webp";
    for (int i = 0; i < 8; i++) {
      ImageList[1][i].filepath = "ChessTextures/bpawn.webp";
      ImageList[6][i].filepath = "ChessTextures/wpawn.webp";
    }

    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (i == 0 || i == 1 || i == 6 || i == 7) {
          ImageList[i][j].currentPiece =
              addImage(c, ImageList[i][j].xposition, ImageList[i][j].yposition,
                       sizeSquares, sizeSquares, &ImageList[i][j].filepath[0]);
          string temp;
          string final;
          regex reg("ChessTextures/");
          regex_replace(back_inserter(temp), ImageList[i][j].filepath.begin(),
                        ImageList[i][j].filepath.end(), reg, "");
          regex reg2("\\..*");
          regex_replace(back_inserter(final), temp.begin(), temp.end(), reg2,
                        "");
          ImageList[i][j].color = final[0];
          final.erase(0, 1);
          ImageList[i][j].typeOfPiece = final;
          cout << ImageList[i][j].typeOfPiece << endl
               << ImageList[i][j].color << endl;
        }
      }
    }
    //======================Text Board==========================//

    MultiText* letters =
        c->addLayer(new MultiText(c, tab->getDefaultStyle(), 0, 0, 0));
    //========================letters==================//
    letters->add(xstart + sizeSquares * 0.3, ystart + sizeSquares * 8.4,
                 c->getStyle()->f, "a");
    letters->add(xstart + sizeSquares * 1.3, ystart + sizeSquares * 8.4,
                 c->getStyle()->f, "b");
    letters->add(xstart + sizeSquares * 2.3, ystart + sizeSquares * 8.4,
                 c->getStyle()->f, "c");
    letters->add(xstart + sizeSquares * 3.3, ystart + sizeSquares * 8.4,
                 c->getStyle()->f, "d");
    letters->add(xstart + sizeSquares * 4.3, ystart + sizeSquares * 8.4,
                 c->getStyle()->f, "e");
    letters->add(xstart + sizeSquares * 5.3, ystart + sizeSquares * 8.4,
                 c->getStyle()->f, "f");
    letters->add(xstart + sizeSquares * 6.3, ystart + sizeSquares * 8.4,
                 c->getStyle()->f, "g");
    letters->add(xstart + sizeSquares * 7.3, ystart + sizeSquares * 8.4,
                 c->getStyle()->f, "h");

    //==========================Numbers=========================//

    letters->add(xstart - sizeSquares * 0.5, ystart + sizeSquares * 7.7,
                 c->getStyle()->f, "1");
    letters->add(xstart - sizeSquares * 0.5, ystart + sizeSquares * 6.7,
                 c->getStyle()->f, "2");
    letters->add(xstart - sizeSquares * 0.5, ystart + sizeSquares * 5.7,
                 c->getStyle()->f, "3");
    letters->add(xstart - sizeSquares * 0.5, ystart + sizeSquares * 4.7,
                 c->getStyle()->f, "4");
    letters->add(xstart - sizeSquares * 0.5, ystart + sizeSquares * 3.7,
                 c->getStyle()->f, "5");
    letters->add(xstart - sizeSquares * 0.5, ystart + sizeSquares * 2.7,
                 c->getStyle()->f, "6");
    letters->add(xstart - sizeSquares * 0.5, ystart + sizeSquares * 1.7,
                 c->getStyle()->f, "7");
    letters->add(xstart - sizeSquares * 0.5, ystart + sizeSquares * 0.7,
                 c->getStyle()->f, "8");
  }

  void GetPosition(GLWin* w) {
    xpos = w->mouseX;
    ypos = w->mouseY;
  }

  void update() {
    if (hasclicked) {
      c->removeLayer(selectedPiece);
      selectedPiece = addImage(c, window->mouseX, window->mouseY, sizeSquares,
                               sizeSquares, &selectedPath[0]);
    }
  }

  void press(GLWin* w) {
    GetPosition(w);
    checkSquare(w);
    if (checkBounds()) {
      if (clickmove) {
        if (clickPiece == nullptr) {
          return;
        } else if (ImageList[selectedRow][selectedColumn].currentPiece ==
                   nullptr) {
          c->removeLayer(ImageList[previousRow][previousColumn].currentPiece);
          clearSquare(previousRow, previousColumn);
          updateSquare(selectedRow, selectedColumn);
          clearSelected();
          clickPiece = nullptr;
          clickmove = false;
          hasclicked = false;
          changeTurn();
          return;
        } else if (ImageList[selectedRow][selectedColumn].currentPiece !=
                       nullptr &&
                   ImageList[selectedRow][selectedColumn].color == turn) {
          clearSelected();
          clickPiece = nullptr;
          clickmove = false;
          hasclicked = false;
        }
      }
      if (!hasclicked && ImageList[selectedRow][selectedColumn].color == turn) {
        if (ImageList[selectedRow][selectedColumn].currentPiece != nullptr) {
          updateSelected(selectedRow, selectedColumn);
          clearSquare(selectedRow, selectedColumn);
          hasclicked = true;
          previousRow = selectedRow;
          previousColumn = selectedColumn;
        }
      }
      PreviousSquare[0][0] = &ImageList[selectedRow][selectedColumn];
    }
  }

  void release(GLWin* w) {
    GetPosition(w);
    if (checkBounds()) {
      checkSquare(w);
      if (selectedPiece == nullptr) {
        return;
      } else if (ImageList[selectedRow][selectedColumn].currentPiece ==
                 nullptr) {
        c->removeLayer(selectedPiece);
        updateSquare(selectedRow, selectedColumn);
        if (ImageList[selectedRow][selectedColumn].xposition ==
                PreviousSquare[0][0]->xposition &&
            ImageList[selectedRow][selectedColumn].yposition ==
                PreviousSquare[0][0]->yposition) {
          clickmove = true;
          clickPiece = ImageList[selectedRow][selectedColumn].currentPiece;
          selectedPiece = nullptr;
          hasclicked = false;
          return;
        }
        clearSelected();
        changeTurn();
        hasclicked = false;
      } else if (ImageList[selectedRow][selectedColumn].currentPiece !=
                 nullptr) {
        c->removeLayer(selectedPiece);
        updateSquare(previousRow, previousColumn);
        hasclicked = false;
        clickmove = false;
        clearSelected();
      }
    } else if (hasclicked && !checkBounds()) {
      c->removeLayer(selectedPiece);
      updateSquare(previousRow, previousColumn);
      hasclicked = false;
      clickmove = false;
      clearSelected();
    }
  }

  void changeTurn() {
    if (turn == 'w') {
      turn = 'b';
    } else {
      turn = 'w';
    }
  }
  void clearSquare(int row, int column) {
    ImageList[row][column].currentPiece = nullptr;
    ImageList[row][column].filepath = "";
    ImageList[row][column].typeOfPiece = "";
    ImageList[row][column].color = '\0';
  }

  void updateSquare(int row, int column) {
    ImageList[row][column].currentPiece = addImage(
        c, ImageList[row][column].xposition, ImageList[row][column].yposition,
        sizeSquares, sizeSquares, &selectedPath[0]);
    ImageList[row][column].filepath = selectedPath;
    ImageList[row][column].typeOfPiece = selectedTypeOfPiece;
    ImageList[row][column].color = selectedColor;
  }

  void updateSelected(int row, int column) {
    selectedPiece = ImageList[row][column].currentPiece;
    selectedPath = ImageList[row][column].filepath;
    selectedTypeOfPiece = ImageList[row][column].typeOfPiece;
    selectedColor = ImageList[row][column].color;
  }

  void clearSelected() {
    selectedPiece = nullptr;
    selectedPath = "";
    selectedTypeOfPiece = "";
    selectedColor = '\0';
  }

  bool checkBounds() {
    if (xpos >= xstart && xpos <= xstart + width && ypos >= ystart &&
        ypos <= ystart + height) {
      return true;
    } else {
      return false;
    }
  }

  void checkSquare(GLWin* w) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (xpos >= ImageList[i][j].xleft && xpos <= ImageList[i][j].xright &&
            ypos <= ImageList[i][j].ybot && ypos >= ImageList[i][j].ytop) {
          selectedRow = i;
          selectedColumn = j;
          return;
        }
      }
    }
  }

  void calculateSquares(float xstart, float ystart, float sizeSquares) {
    uint32_t multiplier = 0;
    for (int i = 0; i < 8; i++) {
      multiplier = 0;
      for (int j = 0; j < 8; j++) {
        ImageList[i][j].xposition = xstart + sizeSquares * multiplier;
        ImageList[i][j].xleft = xstart + sizeSquares * multiplier;
        ImageList[i][j].xright = xstart + sizeSquares * (multiplier + 1);
        multiplier++;
      }
    }
    multiplier = 0;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        ImageList[i][j].yposition = ystart + sizeSquares * multiplier;
        ImageList[i][j].ytop = ystart + sizeSquares * multiplier;
        ImageList[i][j].ybot = ystart + sizeSquares * (multiplier + 1);
      }
      multiplier++;
    }
  }

  Image* addImage(MainCanvas* c, float xstart, float ystart, float width,
                  float height, const char* filePath) {
    Image* m =
        c->addLayer(new Image(c, xstart, ystart, width, height, filePath));
    m->init();
    return m;
  }

  void removeImage(Tab* tab, MainCanvas* c, Image* imageHandle) {
    c->removeLayer(imageHandle);
    tab->getParentWin()->setUpdate();
  }
#if 0
  void possibleMoves(int row, int column, string piece, char color) {
    if (piece == "pawn") {
      checkPawn(row, column, color);
    }
  }
  void checkPawn(int row, int column, char color) {
    if (color == "w" && row == 6) {
      possibleSquares.push_back(Square(row - 1, column));
      possibleSquares.push_back(Square(row - 2, column));
    }
    if (color == "b" && row == 1) {
      possibleSquares.push_back(Square(row + 1, column));
      possibleSquares.push_back(Square(row + 2, column));
    }
    for (int i = 0; i < possibleSquares.size(); i++) {
      if (ImageList[possibleSquares[i].row][possibleSquares[i].column]
              .currentPiece != nullptr) {
        possibleSquares.erase(i, possibleSquares.size() - 1);
        return;
      }
    }
  }
  void cleanRules() { possibleSquares.clear(); }
#endif
};

#pragma once

#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"
#include "opengl/Shape.hh"

class BoardState {  // TODO: Do you parent from shape? Or make asomething?
  // can you share information between games? Only time will tell
  //  which player's turn is it?
  //  score?
  //  player contact for network?
  // observers
 protected:
  //=============== MODEL VARIABLES =========//
  struct squareLocation {
    float xposition;
    float yposition;
    float xleft;
    float ytop;
    float xright;
    float ybot;
    Image* currentPiece = nullptr;
    std::string filepath;
    std::string typeOfPiece;
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
  std::string selectedPath;
  std::string selectedTypeOfPiece;
  std::string selectedPathClick;
  char selectedColor;
  char turn = 'w';
  std::vector<Square> possibleSquares;

 public:
  // virtual void save(const char filename[]) const = 0;
  // virtual void load(const char filename[]);
  // void update();  // get message from server and update this position
  // void move();    // this depends on what kind of game see chess and go
  float updatesizeSquares(float newsizeSquares) {
    sizeSquares = newsizeSquares;
    return sizeSquares;
  }
};

class GrailGameBoard : protected BoardState {
 private:
  BoardState* board;

 public:
  GrailGameBoard(BoardState* board);
  /*virtual void initialBoard(MainCanvas* c, GLWin* window, Tab* tab,
                            float xstart, float ystart, float w, float h) =
     0;*/
  // TODO: Implement override functions
  Image* addImage(MainCanvas* c, float xstart, float ystart, float width,
                  float height, const char* filePath) {
    Image* m =
        c->addLayer(new Image(c, xstart, ystart, width, height, filePath));
    m->init();
    return m;
  }
};

class GrailChessBoard
    : private GrailGameBoard {  // TODO: Inherit from grailgameboard (what
                                // attributes
                                // would be useful to have in all boards?)
 public:
  GrailChessBoard(MainCanvas* c, GLWin* window, Tab* tab, float xstart,
                  float ystart, float w, float h, BoardState* board)
      : GrailGameBoard(board) {
    initialBoard(c, window, tab, xstart, ystart, w, h);
  };

  void initialBoard(MainCanvas* c, GLWin* window, Tab* tab, float xstart,
                    float ystart, float w, float h);

  void caculateSquares(float xstart, float ystart, float sizeSquares) {
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
};

class ChessGame : protected BoardState {
  // chess is usually hardcoded to 8x8
 private:
  GrailChessBoard* drawboard;

 public:
  ChessGame(Tab* tab, GLWin* w);
  ~ChessGame();
  // ChessGame(const ChessGame& orig) = delete;
  // ChessGame& operator=(const ChessGame& orig) = delete;
  // void save(const char filename[]) const;
  /*void load(const char filename[]);
  bool isLegalMove(int from, int to) {
    return true;
  }  // TODO: for now, everything is legal
  void makeMove(int from, int to);
  int getPieceAt(int pos) const;*/
};

#include "opengl/Image.hh"

/**
 * @brief Stored program data
 *
 */
class SquareVariables {
 private:
  struct SquareLocation {
    float xposition;
    float yposition;
    float xleft;
    float ytop;
    float xright;
    float ybot;
    Image* currentPiece = nullptr;
    /*std::string filepath;
    std::string typeOfPiece;*/
  };

 public:
  // dynamically allocate Image** imageList = new Image*[size*size];

  SquareLocation imageList[8][8];

  Image* operator()(int r, int c) { return imageList[size * r + c]; }

  // TODO: where do we get sizeSquares?? Init for chess?
  SquareVariables(float xstart, float ystart, float sizeSquares);
  SquareLocation getSquareInfo(int i, int j);
  void setSquareInfo(int i, int j, SquareLocation newImageList);
  void calculateSquares(float xstart, float ystart, float sizeSquares);
};

class History {
 private:
  SquareVariables from;
  // Location a piece was moved from
  SquareVariables to;
  // Location the piece was moved to
  Piece killed_piece;
  // remembers what piece was killed

  // on this move
  History* next;
  History* prev;

 public:
  History(SquareVariables f, SquareVariables t, Board* b, History* p);
  void setNext(History* n);
  void undoLastMove();
  void redoUndoneMove();
};

/**
 * @brief represent the playing board.
 * Keep track of which pieces are in which positions at a given moment.
 * Also maintain the history of moves
 */

class Board {
 private:
  // Piece* the_board[8][8];
  Color turn;
  History* first_history;
  History* last_history;
  // SquareVariables en_passant;

 public:
  Board();  // creates board with pieces in initial configuration; white's turn
  Piece* pieceAt(SquareVariables p);  // return the piece at location p
  Piece* pieceAt(int x, int y);       // return piece at location x, y
  void placePieceAt(Piece* p,
                    SquareVariables* pt);  // place piece p at location pt
  void move(SquareVariables p1, SquareVariables p2);  // move piece at p1 to p2
  void tryToMove(SquareVariables p1,
                 SquareVariables p2);  // Check wheter move is legal, then move
  // SquareVariables GetEnPassant();
};

class Piece {
 private:
  // Board* board;
  Color color;

 protected:
  virtual MoveType CanMove(SquareVariables p);
  // MoveType can be ILLEGAL, NORMAL, CASTLE, DOUBLESTEP, ENPASSANT
 public:
  Piece(SquareVariables p, Color c, Board* b);
  SquareVariables GetLocation();
  // Color GetColor();
  void setLocation(SquareVariables p);
  void setLocation(int x, int y);
  virtual void tryToMove(SquareVariables p);
};

/**
 * @brief Color class takes care of turns and assigns color to pieces
 *
 */

// OLD SCHOOL:
/*enum Color { black = 0, white = 1 };

Color x = 5;
x++;*/

// NEW METHOD:
/*enum class Color { black = 0, white = 1 };
Color x = Color::black;
Color y = Color(0);*/

class Color {
 private:
  char turn;

 public:
  Color();
  char getTurn();
  void setTurn(char currentTurn);
};

/**
 * @brief checks the type of move which is being carried out and returns the
 * type
 *
 */
class MoveType {};
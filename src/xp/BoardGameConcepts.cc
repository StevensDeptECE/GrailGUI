/*
Suggestion for designs for game boards using Model-View pattern
DBK
*/

class BoardState {  // TODO: Do you parent from shape? Or make asomething?
  // can you share information between games? Only time will tell
  //  which player's turn is it?
  //  score?
  //  player contact for network?
  // observers
 public:
  virtual void save(const char filename[]) const = 0;
  virtual void load(const char filename[]);
  void update();  // get message from server and update this position
  void move();    // this depends on what kind of game see chess and go
};

/*
  Draw a Board State on a Grail window
*/
class GrailGameBoard : public Shape {
 private:
  BoardState* board;

 public:
  GrailGameBoard(BoardState* board);
};

/*
  Represent the state of a chess board with the ability to make moves, save to
  file, communicate with network...
*/
class ChessGame : public BoardState {
  // chess is usually hardcoded to 8x8
 public:
  ChessGame();
  ~ChessGame();
  ChessGame(const ChessGame& orig) = delete;
  ChessGame& operator=(const ChessGame& orig) = delete;
  void save(const char filename[]) const;
  void load(const char filename[]);
  bool isLegalMove(int from, int to) {
    return true;
  }  // TODO: for now, everything is legal
  void makeMove(int from, int to);
  int getPieceAt(int pos) const;
};

/*
  ChessBoard is a graphical observer of chessgame
*/
class GrailChessBoard : GrailGameBoard {
 public:
  GrailChessBoard(
      ChessGame* game) void display() const;  // draw the board on the screen
};

/*
  TextChessBoard is a graphical observer of chessgame
*/
class TextChessBoard : TextBoard {
 public:
  TextChessBoard(
      ChessGame* game) void display() const;  // draw the board in ASCII
};

class GoGame : public Board {
 private:
  uint32_t sizeX, sizeY;

 public:
  void save(const char filename[]) const;
  void load(const char filename[]);
  bool isLegalMove(int pos) {
    return true;
  }  // TODO: for now, everything is legal
  void makeMove(int from, int to);
  void move(int pos);
};

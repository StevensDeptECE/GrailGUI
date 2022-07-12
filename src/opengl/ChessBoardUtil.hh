#include <stdlib.h>

#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"
#include "opengl/MultiText.hh"

using namespace std;
using namespace grail;
// use same click method as testimagemove, make function that checks what struct
// its for in imagelist, then according to that fuck with the image
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
  };

  float xpos;
  float ypos;
  squareLocation ImageList[8][8];
  squareLocation* PreviousSquare[1][1];
  GLWin* window;
  Image* selectedpiece = nullptr;
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
  string selectedPathClick;

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
        }
      }
    }

    //  Black Pieces//
    /*ImageList[0][0].currentPiece =
        addImage(c, ImageList[0][0].xposition, ImageList[0][0].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/brook.webp");
    ImageList[0][1].currentPiece =
        addImage(c, ImageList[0][1].xposition, ImageList[0][1].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bknight.webp");
    Image* bbishop1 =
        addImage(c, ImageList[0][2].xposition, ImageList[0][2].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bbishop.webp");
    Image* bqueen =
        addImage(c, ImageList[0][3].xposition, ImageList[0][3].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bqueen.webp");
    Image* bking =
        addImage(c, ImageList[0][4].xposition, ImageList[0][4].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bking.webp");
    Image* bbishop2 =
        addImage(c, ImageList[0][5].xposition, ImageList[0][5].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bbishop.webp");
    Image* bknight2 =
        addImage(c, ImageList[0][6].xposition, ImageList[0][6].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bknight.webp");
    Image* brook2 =
        addImage(c, ImageList[0][7].xposition, ImageList[0][7].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/brook.webp");
    Image* bpawn1 =
        addImage(c, ImageList[1][0].xposition, ystart + sizeSquares,
                 sizeSquares, sizeSquares, "ChessTextures/bpawn.webp");
    Image* bpawn2 =
        addImage(c, ImageList[1][1].xposition, ImageList[1][1].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bpawn.webp");
    Image* bpawn3 =
        addImage(c, ImageList[1][2].xposition, ImageList[1][2].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bpawn.webp");
    Image* bpawn4 =
        addImage(c, ImageList[1][3].xposition, ImageList[1][3].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bpawn.webp");
    Image* bpawn5 =
        addImage(c, ImageList[1][4].xposition, ImageList[1][4].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bpawn.webp");
    Image* bpawn6 =
        addImage(c, ImageList[1][5].xposition, ImageList[1][5].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bpawn.webp");
    Image* bpawn7 =
        addImage(c, ImageList[1][6].xposition, ImageList[1][6].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bpawn.webp");
    Image* bpawn8 =
        addImage(c, ImageList[1][7].xposition, ImageList[1][6].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/bpawn.webp");
    // White Pieces//
    Image* wrook1 = c->addLayer(
        new Image(c, ImageList[7][0].xposition, ImageList[7][0].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wrook.webp"));
    Image* wknight1 = c->addLayer(
        new Image(c, ImageList[7][1].xposition, ImageList[7][1].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wknight.webp"));
    Image* wbishop1 = c->addLayer(
        new Image(c, ImageList[7][2].xposition, ImageList[7][2].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wbishop.webp"));
    Image* wqueen = c->addLayer(
        new Image(c, ImageList[7][3].xposition, ImageList[7][3].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wqueen.webp"));
    Image* wking = c->addLayer(
        new Image(c, ImageList[7][4].xposition, ImageList[7][4].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wking.webp"));
    Image* wbishop2 = c->addLayer(
        new Image(c, ImageList[7][5].xposition, ImageList[7][5].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wbishop.webp"));
    Image* wknight2 = c->addLayer(
        new Image(c, ImageList[7][6].xposition, ImageList[7][6].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wknight.webp"));
    Image* wrook2 = c->addLayer(
        new Image(c, ImageList[7][7].xposition, ImageList[7][7].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wrook.webp"));
    Image* wpawn1 = c->addLayer(
        new Image(c, ImageList[6][0].xposition, ImageList[6][0].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
    Image* wpawn2 = c->addLayer(
        new Image(c, ImageList[6][1].xposition, ImageList[6][1].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
    Image* wpawn3 = c->addLayer(
        new Image(c, ImageList[6][2].xposition, ImageList[6][2].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
    Image* wpawn4 = c->addLayer(
        new Image(c, ImageList[6][3].xposition, ImageList[6][3].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
    Image* wpawn5 = c->addLayer(
        new Image(c, ImageList[6][4].xposition, ImageList[6][4].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
    Image* wpawn6 = c->addLayer(
        new Image(c, ImageList[6][5].xposition, ImageList[6][5].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
    Image* wpawn7 = c->addLayer(
        new Image(c, ImageList[6][6].xposition, ImageList[6][6].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
    Image* wpawn8 = c->addLayer(
        new Image(c, ImageList[6][7].xposition, ImageList[6][7].yposition,
                  sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
*/
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
      c->removeLayer(selectedpiece);
      cout << "problem"
           << "\n";
      selectedpiece = addImage(c, window->mouseX, window->mouseY, sizeSquares,
                               sizeSquares, &selectedPath[0]);
    }
  }

  void press(GLWin* w) {
    GetPosition(w);
    checkSquare(w);
    if (xpos >= xstart && xpos <= xstart + width && ypos >= ystart &&
        ypos <= ystart + height) {
      if (clickmove) {
        if (clickPiece == nullptr) {
          return;
        } else if (ImageList[selectedRow][selectedColumn].currentPiece ==
                   nullptr) {
          cout << "problem2" << endl;
          selectedPath = selectedPathClick;
          c->removeLayer(ImageList[previousRow][previousColumn].currentPiece);
          ImageList[previousRow][previousColumn].currentPiece = nullptr;
          ImageList[previousRow][previousColumn].filepath = "";
          ImageList[selectedRow][selectedColumn].currentPiece =
              addImage(c, ImageList[selectedRow][selectedColumn].xposition,
                       ImageList[selectedRow][selectedColumn].yposition,
                       sizeSquares, sizeSquares, &selectedPath[0]);
          ImageList[selectedRow][selectedColumn].filepath = selectedPath;
          clickmove = false;
          hasclicked = false;
        }
      }
      if (!hasclicked) {
        if (ImageList[selectedRow][selectedColumn].currentPiece != nullptr) {
          selectedpiece = ImageList[selectedRow][selectedColumn].currentPiece;
          ImageList[selectedRow][selectedColumn].currentPiece = nullptr;
          selectedPath = ImageList[selectedRow][selectedColumn].filepath;
          ImageList[selectedRow][selectedColumn].filepath = "";
          hasclicked = true;
        }
      }
      PreviousSquare[0][0] = &ImageList[selectedRow][selectedColumn];
    }
  }

  void release(GLWin* w) {
    GetPosition(w);
    if (xpos >= xstart && xpos <= xstart + width && ypos >= ystart &&
        ypos <= ystart + height) {
      checkSquare(w);
      if (selectedpiece == nullptr) {
        return;
      } else if (ImageList[selectedRow][selectedColumn].currentPiece ==
                 nullptr) {
        c->removeLayer(selectedpiece);
        ImageList[selectedRow][selectedColumn].currentPiece =
            addImage(c, ImageList[selectedRow][selectedColumn].xposition,
                     ImageList[selectedRow][selectedColumn].yposition,
                     sizeSquares, sizeSquares, &selectedPath[0]);
        clickPiece = selectedpiece;
        selectedPathClick = selectedPath;
        previousRow = selectedRow;
        previousColumn = selectedColumn;
        // selectedpiece = nullptr;
        ImageList[selectedRow][selectedColumn].filepath = selectedPath;
        // selectedPath = "";
        hasclicked = false;
        if (ImageList[selectedRow][selectedColumn].xposition ==
                PreviousSquare[0][0]->xposition &&
            ImageList[selectedRow][selectedColumn].yposition ==
                PreviousSquare[0][0]->yposition) {
          clickmove = true;
        }
      }
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
};
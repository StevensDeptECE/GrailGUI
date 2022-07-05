#include <stdlib.h>

#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"
#include "opengl/MultiText.hh"

using namespace std;
using namespace grail;

class ChessBoard {
 private:
  struct squareLocation {
    float xposition;
    float yposition;
  };

  squareLocation ImageList[8][8];

 public:
  ChessBoard(MainCanvas* c, Tab* tab, float xstart, float ystart, float w,
             float h) {
    float sizeSquares = w / 8;
    calculateSquares(xstart, ystart, sizeSquares);

    //=============================Chess Board Initialization=====///
    StyledMultiShape2D* board =
        c->addLayer(new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0));
    board->fillDrawRectangle(xstart * 0.55, ystart * 0.55, w * 1.15, h * 1.15,
                             gray, black);
    board->CheckeredGrid(xstart, ystart, w, h, 8, 8, black, darkgreen,
                         lightgrey);

    //  Black Pieces//
    Image* brook1 =
        addImage(c, ImageList[0][0].xposition, ImageList[0][0].yposition,
                 sizeSquares, sizeSquares, "ChessTextures/brook.webp");
    Image* bknight1 =
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

    ///================Cool Delete==========//
  }

  void calculateSquares(float xstart, float ystart, float sizeSquares) {
    uint32_t multiplier = 0;
    for (int i = 0; i < 8; i++) {
      multiplier = 0;
      for (int j = 0; j < 8; j++) {
        ImageList[i][j].xposition = xstart + sizeSquares * multiplier;
        multiplier++;
      }
    }
    multiplier = 0;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        ImageList[i][j].yposition = ystart + sizeSquares * multiplier;
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

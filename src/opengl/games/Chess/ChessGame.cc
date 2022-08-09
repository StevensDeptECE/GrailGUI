#include <stdlib.h>

#include <regex>

#include "opengl/Image.hh"
#include "opengl/MultiText.hh"
#include "opengl/games/GrailGameBoard.hh"

using namespace std;
using namespace grail;
// Model View Controller outline for chessgame and future games, inherit from
// GrailGameBoard which holds important attributes shared amongst board games

//============== VIEW ================//
void GrailChessBoard::initialBoard(MainCanvas* c, GLWin* window, Tab* tab,
                                   float xstart, float ystart, float w,
                                   float h) {
  sizeSquares = w / 8;
  caculateSquares(xstart, ystart, sizeSquares);

  //=============================Chess Board Initialization=====///
  StyledMultiShape2D* board =
      c->addLayer(new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0));
  board->fillDrawRectangle(xstart * 0.55, ystart * 0.55, w * 1.15, h * 1.15,
                           gray, black);
  board->CheckeredGrid(xstart, ystart, w, h, 8, 8, black, darkgreen, lightgrey);

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
        regex_replace(back_inserter(final), temp.begin(), temp.end(), reg2, "");
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

//============== CONTROLLER =========//
ChessGame::ChessGame(Tab* tab, GLWin* w) {
  drawboard->initialBoard(c, w, tab, 100, 100, 600, 600);
}

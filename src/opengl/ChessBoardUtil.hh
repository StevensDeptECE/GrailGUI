#include "opengl/GrailGUI.hh"
#include "opengl/Image.hh"

using namespace std;
using namespace grail;

void ChessBoard(Tab* tab, float xstart, float ystart, float w, float h) {
  float sizeSquares = w / 8;

  MainCanvas* c = tab->getMainCanvas();
  StyledMultiShape2D* board =
      c->addLayer(new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0));
  board->CheckeredGrid(xstart, ystart, w, h, 8, 8, black, darkgreen, lightgrey);

  //  Black Pieces//
  Image* brook1 = c->addLayer(new Image(
      c, xstart, ystart, sizeSquares, sizeSquares, "ChessTextures/brook.webp"));
  Image* bknight1 =
      c->addLayer(new Image(c, xstart + (sizeSquares), ystart, sizeSquares,
                            sizeSquares, "ChessTextures/bknight.webp"));
  Image* bbishop1 =
      c->addLayer(new Image(c, xstart + (sizeSquares * 2), ystart, sizeSquares,
                            sizeSquares, "ChessTextures/bbishop.webp"));
  Image* bqueen =
      c->addLayer(new Image(c, xstart + (sizeSquares * 3), ystart, sizeSquares,
                            sizeSquares, "ChessTextures/bqueen.webp"));
  Image* bking =
      c->addLayer(new Image(c, xstart + (sizeSquares * 4), ystart, sizeSquares,
                            sizeSquares, "ChessTextures/bking.webp"));
  Image* bbishop2 =
      c->addLayer(new Image(c, xstart + (sizeSquares * 5), ystart, sizeSquares,
                            sizeSquares, "ChessTextures/bbishop.webp"));
  Image* bknight2 =
      c->addLayer(new Image(c, xstart + (sizeSquares * 6), ystart, sizeSquares,
                            sizeSquares, "ChessTextures/bknight.webp"));
  Image* brook2 =
      c->addLayer(new Image(c, xstart + (sizeSquares * 7), ystart, sizeSquares,
                            sizeSquares, "ChessTextures/brook.webp"));
  Image* bpawn1 =
      c->addLayer(new Image(c, xstart, ystart + sizeSquares, sizeSquares,
                            sizeSquares, "ChessTextures/bpawn.webp"));
  Image* bpawn2 = c->addLayer(
      new Image(c, xstart + sizeSquares, ystart + sizeSquares, sizeSquares,
                sizeSquares, "ChessTextures/bpawn.webp"));
  Image* bpawn3 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 2), ystart + sizeSquares,
                sizeSquares, sizeSquares, "ChessTextures/bpawn.webp"));
  Image* bpawn4 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 3), ystart + sizeSquares,
                sizeSquares, sizeSquares, "ChessTextures/bpawn.webp"));
  Image* bpawn5 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 4), ystart + sizeSquares,
                sizeSquares, sizeSquares, "ChessTextures/bpawn.webp"));
  Image* bpawn6 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 5), ystart + sizeSquares,
                sizeSquares, sizeSquares, "ChessTextures/bpawn.webp"));
  Image* bpawn7 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 6), ystart + sizeSquares,
                sizeSquares, sizeSquares, "ChessTextures/bpawn.webp"));
  Image* bpawn8 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 7), ystart + sizeSquares,
                sizeSquares, sizeSquares, "ChessTextures/bpawn.webp"));

  // White Pieces//
  Image* wrook1 =
      c->addLayer(new Image(c, xstart, ystart + sizeSquares * 7, sizeSquares,
                            sizeSquares, "ChessTextures/wrook.webp"));
  Image* wknight1 = c->addLayer(
      new Image(c, xstart + sizeSquares, ystart + sizeSquares * 7, sizeSquares,
                sizeSquares, "ChessTextures/wknight.webp"));
  Image* wbishop1 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 2), ystart + sizeSquares * 7,
                sizeSquares, sizeSquares, "ChessTextures/wbishop.webp"));
  Image* wqueen = c->addLayer(
      new Image(c, xstart + (sizeSquares * 3), ystart + sizeSquares * 7,
                sizeSquares, sizeSquares, "ChessTextures/wqueen.webp"));
  Image* wking = c->addLayer(
      new Image(c, xstart + (sizeSquares * 4), ystart + sizeSquares * 7,
                sizeSquares, sizeSquares, "ChessTextures/wking.webp"));
  Image* wbishop2 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 5), ystart + sizeSquares * 7,
                sizeSquares, sizeSquares, "ChessTextures/wbishop.webp"));
  Image* wknight2 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 6), ystart + sizeSquares * 7,
                sizeSquares, sizeSquares, "ChessTextures/wknight.webp"));
  Image* wrook2 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 7), ystart + sizeSquares * 7,
                sizeSquares, sizeSquares, "ChessTextures/wrook.webp"));
  Image* wpawn1 =
      c->addLayer(new Image(c, xstart, ystart + sizeSquares * 6, sizeSquares,
                            sizeSquares, "ChessTextures/wpawn.webp"));
  Image* wpawn2 = c->addLayer(
      new Image(c, xstart + sizeSquares, ystart + sizeSquares * 6, sizeSquares,
                sizeSquares, "ChessTextures/wpawn.webp"));
  Image* wpawn3 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 2), ystart + sizeSquares * 6,
                sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
  Image* wpawn4 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 3), ystart + sizeSquares * 6,
                sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
  Image* wpawn5 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 4), ystart + sizeSquares * 6,
                sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
  Image* wpawn6 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 5), ystart + sizeSquares * 6,
                sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
  Image* wpawn7 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 6), ystart + sizeSquares * 6,
                sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
  Image* wpawn8 = c->addLayer(
      new Image(c, xstart + (sizeSquares * 7), ystart + sizeSquares * 6,
                sizeSquares, sizeSquares, "ChessTextures/wpawn.webp"));
}
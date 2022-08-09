#include "opengl/games/Chess/ChessClasses.hh"

SquareVariables::SquareVariables(float xstart, float ystart,
                                 float sizeSquares) {
  calculateSquares(xstart, ystart, sizeSquares);
}

void SquareVariables::calculateSquares(float xstart, float ystart,
                                       float sizeSquares) {
  uint32_t multiplier = 0;
  for (int i = 0; i < 8; i++) {
    multiplier = 0;
    for (int j = 0; j < 8; j++) {
      imageList[i][j].xposition = xstart + sizeSquares * multiplier;
      imageList[i][j].xleft = xstart + sizeSquares * multiplier;
      imageList[i][j].xright = xstart + sizeSquares * (multiplier + 1);
      multiplier++;
    }
  }
  multiplier = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      imageList[i][j].yposition = ystart + sizeSquares * multiplier;
      imageList[i][j].ytop = ystart + sizeSquares * multiplier;
      imageList[i][j].ybot = ystart + sizeSquares * (multiplier + 1);
    }
    multiplier++;
  }
}

SquareLocation SquareVariables::getSquareInfo(int i, int j) {
  return imageList[i][j];
}

void SquareVariables::setSquareInfo(int i, int j, squareLocation newImageList) {
  imageList[i][j] = newImageList;
}
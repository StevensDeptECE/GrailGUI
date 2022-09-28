#include "opengl/games/ChessVisual.hh"

using namespace std;
using namespace grail;

void ChessVisual::getPosition(GLWin* w) {
  xpos = w->mouseX;
  ypos = w->mouseY;
}
void ChessVisual::calculateSquares(float xstart, float ystart,
                                   float sizeSquares) {
  int32_t multiplier = 0;
  for (int i = 0; i < 8; i++) {
    multiplier = 0;
    for (int j = 0; j < 8; j++) {
      visual_board[i][j].xposition = xstart + sizeSquares * multiplier;
      visual_board[i][j].xleft = xstart + sizeSquares * multiplier;
      visual_board[i][j].xright = xstart + sizeSquares * (multiplier + 1);
      multiplier++;
    }
  }
  multiplier = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      visual_board[i][j].yposition = ystart + sizeSquares * multiplier;
      visual_board[i][j].ytop = ystart + sizeSquares * multiplier;
      visual_board[i][j].ybot = ystart + sizeSquares * (multiplier + 1);
    }
    multiplier++;
  }
}

Image* ChessVisual::addImage(MainCanvas* c, float xstart, float ystart,
                             float width, float height, const char* filePath) {
  Image* m = new Image(c, xstart, ystart, width, height, filePath);
  return m;
}

void ChessVisual::clearSquare(int row, int column) {
  visual_board[row][column].currentPiece = nullptr;
}
// Called when you drop the piece on the board
void ChessVisual::updateSquare(int row, int column) {
  selectedPiece->translate(selectedxPos * -1, selectedyPos * -1, 0);
  selectedPiece->translate(visual_board[row][column].xposition,
                           visual_board[row][column].yposition, 0);
  visual_board[row][column].currentPiece = selectedPiece;
}

void ChessVisual::updateSelected(int row, int column) {
  selectedPiece = visual_board[row][column].currentPiece;
  selectedxPos = visual_board[row][column].xposition;
  selectedyPos = visual_board[row][column].yposition;
}

void ChessVisual::clearSelected() {
  selectedPiece = nullptr;
  selectedPath = "";
}

bool ChessVisual::checkBounds() {
  if (xpos >= xstart && xpos <= xstart + width && ypos >= ystart &&
      ypos <= ystart + height) {
    return true;
  } else {
    return false;
  }
}

void ChessVisual::checkLocation(GLWin* w) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (xpos >= visual_board[i][j].xleft &&
          xpos <= visual_board[i][j].xright &&
          ypos <= visual_board[i][j].ybot && ypos >= visual_board[i][j].ytop) {
        selectedRow = i;
        selectedColumn = j;
        return;
      }
    }
  }
}

void ChessVisual::setKeyBinds(Tab* tab, GLWin* w) {
  tab->bindEvent(Tab::MOUSE0_PRESS, [w, this]() {
    press(w);
    save();
    load();
  });
  tab->bindEvent(Tab::MOUSE0_RELEASE, [w, this]() { release(w); });
}

ChessVisual::ChessVisual(ChessBoard* chess_pieces, MainCanvas* c, GLWin* window,
                         Tab* tab, float xstart, float ystart, float w, float h)
    : board(c->addLayer(
          new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0))),
      saveButton(c->addLayer(
          new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0))),
      loadButton(c->addLayer(
          new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0))),
      c(c),
      window(window),
      width(w),
      height(h),
      xstart(xstart),
      ystart(ystart),
      chess_pieces(chess_pieces) {
  sizeSquares = w / 8;
  calculateSquares(xstart, ystart, sizeSquares);

  //============Drawing Initial ChessBoard========//
  loadButton->fillDrawRectangle(xstart * 8 - 5, ystart, 125, 70, green, red);
  saveButton->fillDrawRectangle(xstart * 8 - 5, ystart + 100, 125, 70, green,
                                red);
  board->fillDrawRectangle(xstart * 0.55, ystart * 0.55, w * 1.15, h * 1.15,
                           gray, black);
  board->CheckeredGrid(xstart, ystart, w, h, 8, 8, black, darkgreen, lightgrey);
  //===========Drawing Letters=======///
  MultiText* letters =
      c->addLayer(new MultiText(c, tab->getDefaultStyle(), 0, 0, 0));
  letters->add(xstart * 8, ystart + 50, c->getStyle()->f, "load");
  letters->add(xstart * 8, ystart + 150, c->getStyle()->f, "save");

  for (int i = 0; i < 12; i++) {
    switch (i) {
      case 0:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/brook.webp");
        break;
      case 1:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/bknight.webp");
        break;
      case 2:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/bbishop.webp");
        break;
      case 3:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/bqueen.webp");
        break;
      case 4:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/bking.webp");
        break;
      case 5:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/bpawn.webp");
        break;
      case 6:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/wrook.webp");
        break;
      case 7:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/wknight.webp");
        break;
      case 8:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/wbishop.webp");
        break;
      case 9:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/wqueen.webp");
        break;
      case 10:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/wking.webp");
        break;
      case 11:
        piecetype[i] = addImage(c, 0, 0, sizeSquares, sizeSquares,
                                "ChessTextures/wpawn.webp");
        break;
    }
  }

  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      if (chess_pieces->getPiece(i, j) > 0) {
        int8_t piecekind = (chess_pieces->getColor(i, j) * numpieces +
                            chess_pieces->getPiece(i, j)) -
                           1;
        c->addLayer(visual_board[i][j].currentPiece =
                        new Transform(c, piecetype[piecekind]));
        visual_board[i][j].currentPiece->translate(
            visual_board[i][j].xposition, visual_board[i][j].yposition, 0);
      }
    }

  setKeyBinds(tab, window);  // set key binds for program
};

void ChessVisual::clearBoard() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (chess_pieces->getPiece(i, j) > 0) {
        c->removeLayer(visual_board[i][j].currentPiece);
        visual_board[i][j].currentPiece = nullptr;
      }
    }
  }
}

void ChessVisual::redrawBoard() {
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++) {
      if (chess_pieces->getPiece(i, j) > 0) {
        int8_t piecekind = (chess_pieces->getColor(i, j) * numpieces +
                            chess_pieces->getPiece(i, j)) -
                           1;
        c->addLayer(visual_board[i][j].currentPiece =
                        new Transform(c, piecetype[piecekind]));
        visual_board[i][j].currentPiece->translate(
            visual_board[i][j].xposition, visual_board[i][j].yposition, 0);
      }
    }
}

void ChessVisual::load() {
  if (window->mouseX < xstart * 8 + 100 && window->mouseX > xstart * 8 &&
      window->mouseY < ystart + 50 && window->mouseY > ystart) {
    clearBoard();
    chess_pieces->load("Chess.txt");
    redrawBoard();
  }
}

void ChessVisual::save() {
  if (window->mouseX < xstart * 8 + 100 && window->mouseX > xstart * 8 &&
      window->mouseY < ystart + 150 && window->mouseY > ystart + 100) {
    chess_pieces->save("Chess.txt");
  }
}

void ChessVisual::update() {
  if (hasclicked) {
    int8_t piecekind =
        (chess_pieces->getColor(selectedRow, selectedColumn) * numpieces +
         chess_pieces->getPiece(selectedRow, selectedColumn)) -
        1;
    c->removeLayer(selectedPiece);
    c->addLayer(selectedPiece = new Transform(c, piecetype[piecekind]));
    selectedPiece->translate(window->mouseX, window->mouseY, 0);
    selectedxPos = window->mouseX;
    selectedyPos = window->mouseY;
  }
}

void ChessVisual::press(GLWin* w) {
  getPosition(w);
  checkLocation(w);
  if (checkBounds()) {
    if (clickmove) {
      if (clickPiece == nullptr) {
        return;
      } else if (visual_board[selectedRow][selectedColumn].currentPiece ==
                 nullptr) {
        selectedPiece = clickPiece;
        selectedxPos = visual_board[previousRow][previousColumn].xposition;
        selectedyPos = visual_board[previousRow][previousColumn].yposition;
        if (chess_pieces->move(previousColumn, previousRow, selectedColumn,
                               selectedRow)) {
          clearSquare(previousRow, previousColumn);
          updateSquare(selectedRow, selectedColumn);
          clearSelected();
          clickPiece = nullptr;
          selectedPiece = nullptr;
          clickmove = false;
          hasclicked = false;
        } else {
          clearSelected();
          clickPiece = nullptr;
          selectedPiece = nullptr;
          clickmove = false;
          hasclicked = false;
        }
        return;
      } else if (visual_board[selectedRow][selectedColumn].currentPiece !=
                     nullptr &&
                 chess_pieces->getColor(selectedRow, selectedColumn) ==
                     chess_pieces->getTurn()) {
        clearSelected();
        clickPiece = nullptr;
        selectedPiece = nullptr;
        clickmove = false;
        hasclicked = false;
        return;
      } else if (visual_board[selectedRow][selectedColumn].currentPiece !=
                     nullptr &&
                 chess_pieces->getColor(selectedRow, selectedColumn) !=
                     chess_pieces->getTurn()) {
        // click piece delete no rules implementation
        selectedPiece = clickPiece;
        if (chess_pieces->move(previousColumn, previousRow, selectedColumn,
                               selectedRow)) {
          selectedxPos = visual_board[previousRow][previousColumn].xposition;
          selectedyPos = visual_board[previousRow][previousColumn].yposition;
          Transform* pieceRemove =
              visual_board[selectedRow][selectedColumn].currentPiece;
          c->removeLayer(pieceRemove);
          clearSquare(previousRow, previousColumn);
          updateSquare(selectedRow, selectedColumn);
          clearSelected();
          clickPiece = nullptr;
          selectedPiece = nullptr;
          clickmove = false;
          hasclicked = false;
          return;
        } else {
          updateSquare(previousRow, previousColumn);
          clearSelected();
          clickPiece = nullptr;
          selectedPiece = nullptr;
          clickmove = false;
          hasclicked = false;
          return;
        }
      }
    }
    if (!hasclicked && chess_pieces->getColor(selectedRow, selectedColumn) ==
                           chess_pieces->getTurn()) {
      if (visual_board[selectedRow][selectedColumn].currentPiece != nullptr) {
        updateSelected(selectedRow, selectedColumn);
        clearSquare(selectedRow, selectedColumn);
        hasclicked = true;
        previousRow = selectedRow;
        previousColumn = selectedColumn;
      }
    }
    PreviousSquare[0][0] = &visual_board[selectedRow][selectedColumn];
  }
}

void ChessVisual::release(GLWin* w) {
  getPosition(w);
  if (checkBounds()) {
    checkLocation(w);
    if (selectedPiece == nullptr) {
      return;
    } else if (visual_board[selectedRow][selectedColumn].currentPiece ==
               nullptr) {
      if (visual_board[selectedRow][selectedColumn].xposition ==
              PreviousSquare[0][0]->xposition &&
          visual_board[selectedRow][selectedColumn].yposition ==
              PreviousSquare[0][0]->yposition) {
        updateSquare(selectedRow, selectedColumn);
        clickmove = true;
        clickPiece = visual_board[selectedRow][selectedColumn].currentPiece;
        selectedPiece = nullptr;
        hasclicked = false;
        return;
      }
      bool legal = chess_pieces->move(previousColumn, previousRow,
                                      selectedColumn, selectedRow);
      if (legal) {
        updateSquare(selectedRow, selectedColumn);
      } else {
        updateSquare(previousRow, previousColumn);
      }
      clearSelected();
      hasclicked = false;
    } else if (visual_board[selectedRow][selectedColumn].currentPiece !=
               nullptr) {
// DEMO CAPTURE WITHOUT RULE IMPLEMENTATION
#if 1
      if (chess_pieces->getColor(selectedRow, selectedColumn) ==
          chess_pieces->getTurn()) {
        updateSquare(previousRow, previousColumn);
        hasclicked = false;
        clickmove = false;
        clearSelected();
      } else {
        if (chess_pieces->move(previousColumn, previousRow, selectedColumn,
                               selectedRow)) {
          Transform* pieceRemove =
              visual_board[selectedRow][selectedColumn].currentPiece;
          c->removeLayer(pieceRemove);
          updateSquare(selectedRow, selectedColumn);
          hasclicked = false;
          clickmove = false;
          clearSelected();
        } else {
          updateSquare(previousRow, previousColumn);
          clearSelected();
          hasclicked = false;
        }
      }
#endif

#if 0
      // previous implementation where the piece is returned to its original
      // position
      updateSquare(previousRow, previousColumn);
      hasclicked = false;
      clickmove = false;
      clearSelected();
#endif
    }
  } else if (hasclicked && !checkBounds()) {
    updateSquare(selectedRow, selectedColumn);
    hasclicked = false;
    clickmove = false;
    clearSelected();
  }
}

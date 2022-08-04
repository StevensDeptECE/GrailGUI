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
  Image* m = c->addLayer(new Image(c, xstart, ystart, width, height, filePath));
  m->init();
  return m;
}

void ChessVisual::clearSquare(int row, int column) {
  visual_board[row][column].currentPiece = nullptr;
  visual_board[row][column].filepath = "";
}
void ChessVisual::updateSquare(int row, int column) {
  visual_board[row][column].currentPiece =
      addImage(c, visual_board[row][column].xposition,
               visual_board[row][column].yposition, sizeSquares, sizeSquares,
               &selectedPath[0]);
  visual_board[row][column].filepath = selectedPath;
}

void ChessVisual::updateSelected(int row, int column) {
  selectedPiece = visual_board[row][column].currentPiece;
  selectedPath = visual_board[row][column].filepath;
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
  tab->bindEvent(Tab::MOUSE0_PRESS, [w, this]() { press(w); });
  tab->bindEvent(Tab::MOUSE0_RELEASE, [w, this]() { release(w); });
}

ChessVisual::ChessVisual(ChessController* controller, MainCanvas* c,
                         GLWin* window, Tab* tab, float xstart, float ystart,
                         float w, float h)
    : board(c->addLayer(
          new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0))),
      c(c),
      window(window),
      width(w),
      height(h),
      xstart(xstart),
      ystart(ystart),
      controller(controller) {
  sizeSquares = w / 8;
  calculateSquares(xstart, ystart, sizeSquares);

  //============Drawing Initial ChessBoard========//
  board->fillDrawRectangle(xstart * 0.55, ystart * 0.55, w * 1.15, h * 1.15,
                           gray, black);
  board->CheckeredGrid(xstart, ystart, w, h, 8, 8, black, darkgreen, lightgrey);

  // store filepaths//
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (controller->getColor(i, j) == 0 && controller->getPiece(i, j) == 6) {
        visual_board[i][j].filepath = "ChessTextures/bpawn.webp";
      } else if (controller->getColor(i, j) == 1 &&
                 controller->getPiece(i, j) == 6) {
        visual_board[i][j].filepath = "ChessTextures/wpawn.webp";
      } else if (controller->getColor(i, j) == 0 &&
                 controller->getPiece(i, j) == 3) {
        visual_board[i][j].filepath = "ChessTextures/bbishop.webp";
      } else if (controller->getColor(i, j) == 0 &&
                 controller->getPiece(i, j) == 4) {
        visual_board[i][j].filepath = "ChessTextures/bqueen.webp";
      } else if (controller->getColor(i, j) == 0 &&
                 controller->getPiece(i, j) == 5) {
        visual_board[i][j].filepath = "ChessTextures/bking.webp";
      } else if (controller->getColor(i, j) == 0 &&
                 controller->getPiece(i, j) == 2) {
        visual_board[i][j].filepath = "ChessTextures/bknight.webp";
      } else if (controller->getColor(i, j) == 0 &&
                 controller->getPiece(i, j) == 1) {
        visual_board[i][j].filepath = "ChessTextures/brook.webp";
      } else if (controller->getColor(i, j) == 1 &&
                 controller->getPiece(i, j) == 2) {
        visual_board[i][j].filepath = "ChessTextures/wknight.webp";
      } else if (controller->getColor(i, j) == 1 &&
                 controller->getPiece(i, j) == 3) {
        visual_board[i][j].filepath = "ChessTextures/wbishop.webp";
      } else if (controller->getColor(i, j) == 1 &&
                 controller->getPiece(i, j) == 4) {
        visual_board[i][j].filepath = "ChessTextures/wqueen.webp";
      } else if (controller->getColor(i, j) == 1 &&
                 controller->getPiece(i, j) == 5) {
        visual_board[i][j].filepath = "ChessTextures/wking.webp";
      } else if (controller->getColor(i, j) == 1 &&
                 controller->getPiece(i, j) == 1) {
        visual_board[i][j].filepath = "ChessTextures/wrook.webp";
      }
      // add images to board with position variables
      if (visual_board[i][j].filepath != "") {
        visual_board[i][j].currentPiece = addImage(
            c, visual_board[i][j].xposition, visual_board[i][j].yposition,
            sizeSquares, sizeSquares, &visual_board[i][j].filepath[0]);
      }
    }
  }

  setKeyBinds(tab, window);  // set key binds for program
};

void ChessVisual::update() {
  if (hasclicked) {
    c->removeLayer(selectedPiece);
    selectedPiece = addImage(c, window->mouseX, window->mouseY, sizeSquares,
                             sizeSquares, &selectedPath[0]);
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
        c->removeLayer(visual_board[previousRow][previousColumn].currentPiece);
        clearSquare(previousRow, previousColumn);
        updateSquare(selectedRow, selectedColumn);
        clearSelected();
        clickPiece = nullptr;
        clickmove = false;
        hasclicked = false;
        // TODO:MESSAGE TO CONTROLLER TO MAKE MOVE
        controller->setMove(previousColumn, previousRow, selectedColumn,
                            selectedRow);
        return;
      } else if (visual_board[selectedRow][selectedColumn].currentPiece !=
                     nullptr &&
                 controller->getColor(selectedRow, selectedColumn) ==
                     controller->getTurn()) {
        clearSelected();
        clickPiece = nullptr;
        clickmove = false;
        hasclicked = false;
      }
    }
    if (!hasclicked && controller->getColor(selectedRow, selectedColumn) ==
                           controller->getTurn()) {
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
      c->removeLayer(selectedPiece);
      updateSquare(selectedRow, selectedColumn);
      if (visual_board[selectedRow][selectedColumn].xposition ==
              PreviousSquare[0][0]->xposition &&
          visual_board[selectedRow][selectedColumn].yposition ==
              PreviousSquare[0][0]->yposition) {
        clickmove = true;
        clickPiece = visual_board[selectedRow][selectedColumn].currentPiece;
        selectedPiece = nullptr;
        hasclicked = false;
        return;
      }
      clearSelected();
      // TODO:MESSAGE TO CONTROLLER TO MAKE MOVE
      controller->setMove(previousColumn, previousRow, selectedColumn,
                          selectedRow);
      hasclicked = false;
    } else if (visual_board[selectedRow][selectedColumn].currentPiece !=
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

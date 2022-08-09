#include "opengl/GrailGUI.hh"
#include "opengl/Group.hh"
#include "opengl/Image.hh"
#include "opengl/util/Transformation.hh"
#include "util/DynArray.hh"

using namespace std;
using namespace grail;

/*class ImageMove : public Member {
 private:
  bool hasclicked = false;
  float xpos, ypos, width, height;
  Image* piece;
  Transformation* trans;

 public:
  ImageMove(Tab* t, GLWin* w) : Member(t, 0, 0.0125) {
    MainCanvas* c = t->getMainCanvas();
    trans = new Transformation();

    xpos = 100;
    ypos = 100;
    width = 200;
    height = 200;

    piece = addNewImage(xpos, ypos, width, height);
  }

  void update() {
    trans->ident();
    trans->rotate(20, 1, 1, 0);
    piece->render();
  }

  Image* addNewImage(float xpos, float ypos, float width, float height) {
    Image* m = c->addLayer(
        new Image(c, xpos, ypos, width, height, "ChessTextures/brook.webp"));
    m->init();
    return m;
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Transformation Practice");
  w->setSize(1024, 800);
  new ImageMove(t, w);
}*/

class Piece {
 private:
  double xpos;
  double ypos;
  double width;
  double height;
  const char* filePath;
  Transformation* trans;

 public:
  Piece(Canvas* c, const Style* s, double xpos, double ypos, double width,
        double height, const char filePath[]);
  void update();
};

Piece::Piece(Canvas* c, const Style* s, double xpos, double ypos, double width,
             double height, const char filePath[])
    : xpos(xpos), ypos(ypos), width(width), height(height), filePath(filePath) {
  trans = new Transformation();
  Image* piece = c->addLayer(new Image(c, xpos, ypos, width, height, filePath));
  update();
}

void Piece::update() {
  trans->ident();
  // trans->rotate(glm::radians(90.0f), 0, 1, 0);
  trans->scale(10);
}

class RotatePiece : public Member {
 private:
  DynArray<Piece*> Pieces;

 public:
  RotatePiece(Tab* t) : Member(t, 0, .0125), Pieces(20) {
    MainCanvas* c = t->getMainCanvas();
    GLWin* w = tab->getParentWin();
    const Style* s = w->getDefaultStyle();
    Transformation tbishop;
    Piece* bishop;
    /*Pieces.add(bishop = new Piece(c, s, 200, 200, 200, 200,
                                  "ChessTextures/bbishop.webp"));*/
    Image* bishop2 = c->addLayer(
        new Image(c, 200, 200, 200, 200, "ChessTextures/bbishop.webp"));
    tbishop.scale(10);
  }

  void update() {
    double t = tab->time();
    for (int i = 0; i < Pieces.size(); i++) Pieces[i]->update();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Transformation Practice");
  w->setSize(1024, 800);
  new RotatePiece(t);
}

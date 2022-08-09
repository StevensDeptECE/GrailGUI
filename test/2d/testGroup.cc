#include "opengl/Canvas.hh"
#include "opengl/GrailGUI.hh"
#include "opengl/Group.hh"
#include "opengl/Image.hh"
#include "opengl/MultiShape2D.hh"
#include "opengl/util/Transformation.hh"

using namespace std;
using namespace grail;

class TestGroup : public Member {
 private:
  Group* group;
  Image* piece;
  Transformation* trans;
  glm::mat4 transformation;
  StyledMultiShape2D* rect;

 public:
  TestGroup(Tab* tab, MainCanvas* c) : Member(tab, 0, 0.1) {
    piece = c->addLayer(
        new Image(c, 200, 200, 200, 200, "ChessTextures/bbishop.webp"));
    rect =
        c->addLayer(new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0));
    // group->add(piece);
    trans = new Transformation();
    trans->scale(10);
    transformation = trans->getTransform();
    *parentCanvas->getProjection();
    rect =
        c->addLayer(new StyledMultiShape2D(c, tab->getDefaultStyle(), 0, 0, 0));
    rect->fillRectangle(600, 300, 200, 200, green);

    // rect->render(transformation);
    //  piece->render(transformation);
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  w->setTitle("Test group");
  MainCanvas* c = defaultTab->getMainCanvas();
  new TestGroup(defaultTab, c);
}
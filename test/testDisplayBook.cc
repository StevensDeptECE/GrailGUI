#include "opengl/GrailGUI.hh"
#include "opengl/DocView.hh"
#include "opengl/Document.hh"

using namespace std;
using namespace grail;
class BookViewer : public GLWin {
 private:
  Document *doc;
  DocView *docView;
  const char *filename;
  void init();

 public:
  BookViewer(const char filename[]) : filename(filename) {}
  
  static void advance(GLWin *w) { ((BookViewer *)w)->docView->advance(); }
  static void back(GLWin *w) { ((BookViewer *)w)->docView->back(); }

  static void advance10(GLWin *w) { ((BookViewer *)w)->docView->advance10(); }

  static void top(GLWin *w) { ((BookViewer *)w)->docView->top(); }

  static void bottom(GLWin *w) { ((BookViewer *)w)->docView->bottom(); }
};

void BookViewer::init() {
  setAction(1000, bottom);
  setAction(1001, top);
  setAction(1002, advance);
  setAction(1003, back);
  setEvent(264, 1000);
  setEvent(265, 1001);
  setEvent(262, 1002);
  setEvent(263, 1003);

//  Font *font = getDefaultFont();
  const Font* font = FontFace::get("TIMES", 30, 0);
  Style *s = new Style(font, 1, 1, 1, 0, 0, 0);
  s->setLineWidth(1);
  Canvas *c = currentTab()->getMainCanvas();
  c->addLayer(new Image(c, 0, 5, 320, 32, "res/toolbar1.png", s));
  StyledMultiShape2D *m = new StyledMultiShape2D(c, s);
  m->fillTriangle(350, 5, 380, 0, 360, 30, green);
  m->fillRoundRect(400, 5, 950, 40, 10, 10, lightblue);
  m->fillRoundRect(405, 7, 940, 36, 10, 10, lightgrey);
  c->addLayer(m);

  PageLayout layout(10, 70, 1300, 1100, 1360, 40, 10, 40, font);
  doc = new Document(layout);
  doc->appendFile(layout, filename);
  docView = new DocView(c, s, doc);
  docView->update();
  c->addLayer(docView);
  //c->addLayer(new Image(400, 400, 400, 400, "res/trumpmelania.png", s));
}

int main(int argc, char *argv[]) {
  const char *filename = argc < 2 ? "res/Annatest.txt" : argv[1];
  return GLWin::init(new BookViewer(filename), 1600, 1024);
}

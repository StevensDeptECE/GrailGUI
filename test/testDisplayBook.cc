#include "opengl/DocView.hh"
#include "opengl/Document.hh"
#include "opengl/GrailGUI.hh"

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
  auto bottom_fnptr = std::bind(&BookViewer::bottom, this);
  auto top_fnptr = std::bind(&BookViewer::top, this);
  auto advance_fnptr = std::bind(&BookViewer::advance, this);
  auto back_fnptr = std::bind(&BookViewer::back, this);
  register_callback(264, "bottom", Security::SAFE, bottom_fnptr);
  register_callback(265, "top", Security::SAFE, top_fnptr);
  register_callback(262, "advance", Security::SAFE, advance_fnptr);
  register_callback(263, "back", Security::SAFE, back_fnptr);

  //  Font *font = getDefaultFont();
  const Font *font = FontFace::get("TIMES", 30, 0);
  Style *s = new Style(font, 1, 1, 1, 0, 0, 0);
  s->setLineWidth(1);
  Canvas *c = currentTab()->getMainCanvas();
  c->addLayer(new Image(c, 0, 5, 320, 32, "res/toolbar1.png"));
  StyledMultiShape2D *m = new StyledMultiShape2D(c, s);
  m->fillTriangle(350, 5, 380, 0, 360, 30, green);
  m->fillRoundRect(400, 5, 950, 40, 10, 10, lightblue);
  m->fillRoundRect(405, 7, 940, 36, 10, 10, lightgrey);
  c->addLayer(m);

  PageLayout layout(10, 70, 1350, 1100, 1360, 40, 10, 40, font, 1500);
  doc = new Document(layout);
  doc->appendFile(layout, filename);
  docView = new DocView(c, s, doc);
  docView->update();
  c->addLayer(docView);
  // c->addLayer(new Image(400, 400, 400, 400, "res/trumpmelania.png", s));
}

int main(int argc, char *argv[]) {
  const char *filename = argc < 2 ? "res/Annatest.txt" : argv[1];
  return GLWin::init(new BookViewer(filename), 1600, 1024);
}

#include "opengl/Animated.hh"
#include "opengl/DocView.hh"
#include "opengl/Document.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;
class BookViewer : public Animated {
 private:
  Document *doc;
  DocView *docView;
  const char *filename;
  void init();

 public:
	BookViewer(const char filename[]);

  void advance() { docView->advance(); }
  void back() { docView->back(); }
  void advance10() { docView->advance10(); }
  void top() { docView->top(); }
  void bottom() { docView->bottom(); }
};

BookViewer::BookViewer(Tab* tab, const char filename[]) : filename(filename) {
  bindEvent(264, &BookViewer::bottom, this);
  bindEvent(265, &BookViewer::top, this);
  bindEvent(262, &BookViewer::advance, this);
  bindEvent(263, &BookViewer::back, this);

  //  Font *font = getDefaultFont();
  const Font *font = FontFace::get("TIMES", 30, 0);
  Style *s = new Style(font, 1, 1, 1, 0, 0, 0, 1);
  Canvas *c = tab->getMainCanvas();
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

void grailmain(int argc, char *argv[], GLWin* w, Tab* defaultTab) {
  const char *filename = argc < 2 ? "res/Annatest.txt" : argv[1];	
	tab->addAnimated(new BookViewer(defaultTab, filename));
}

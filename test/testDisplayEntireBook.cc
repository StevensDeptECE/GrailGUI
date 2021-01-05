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
#if 0
  void run() override {
		sleep(1);
    for (;;) {
      usleep(20000);
      docView->advance();
    }
  }
#endif
	void update()
	{
		docView->advance();
	}
};

void BookViewer::init() {
	Font *font = getDefaultFont();
	Style *s = new Style(font, 0, 0, 0, 0, 0, 0);
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
}

int main(int argc, char *argv[])
{
	const char *filename = argc < 2 ? "res/prideandprejudice.txt" : argv[1];
	return GLWin::init(new BookViewer(filename), 1450, 1150);
}

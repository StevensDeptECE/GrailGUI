#include <unistd.h>

#include "grail/Grail.hh"
#include "opengl/GLWin.hh"
#include "opengl/primitivesV2/Colors.hh"
#include "opengl/primitivesV2/DocView.hh"
#include "opengl/primitivesV2/Document.hh"
#include "opengl/primitivesV2/StyledMultiShape2D.hh"

using namespace std;
using namespace grail;

class Document;

class TableViewer : public Grail {
 private:
  Document* doc;
  DocView* docView;
  const char* filename;
  void init();
  char* tableData;

 public:
  TableViewer(uint32_t w, uint32_t h, const char filename[])
      : Grail(w, h), filename(filename) {}
};

void TableViewer::init() {
  const Font* font = getDefaultFont();
  Style* s = new Style(font, 0, 0, 0, 0, 0, 0);
  s->setLineWidth(1);
  Canvas* c = currentTab()->getMainCanvas();
  StyledMultiShape2D* m = new StyledMultiShape2D(s);
  m->fillTriangle(350, 5, 380, 0, 360, 30, green);
  m->fillRoundRect(400, 5, 950, 40, 10, 10, lightblue);
  m->fillRoundRect(405, 7, 940, 36, 10, 10, lightgrey);
  c->addLayer(m);

  int fh = ::open(filename, O_RDONLY);
  if (fh < 0) {
    cerr << "can't open file " << filename << '\n';
    exit(-1);
  }
  uint32_t len = 200736;
  tableData = new char[len];
  read(fh, tableData, len);
  close(fh);
  PageLayout layout(10, 70, 1300, 1100, 1360, 40, 10, 40, font);
  doc = new Document(layout);
  docView = new DocView(s, doc);

  docView->update();
  c->addLayer(docView);
}

int main(int argc, char* argv[]) {
  const char* filename = argc < 2 ? "ibm.bin" : argv[1];
  return Grail::init(new TableViewer(1450, 1150, filename), "out.cml");
}

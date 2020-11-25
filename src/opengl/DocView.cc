#include "opengl/DocView.hh"

#include <algorithm>

#include "glad/glad.h"
#include "opengl/Style.hh"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/Shader.hh"
#include "opengl/Colors.hh"
#include "opengl/Document.hh"
#include "opengl/MultiText.hh"

using namespace std;
using namespace grail;

DocView::DocView(const Style* style, Canvas* const c, const Document* doc)
    : style(style), doc(doc), pageNum(0) {
  m = new StyledMultiShape2D(style, 16, 16, 16, 16);
  t = new MultiText(style, 128 * 128);
  c->addLayer(m);  // add the components to the Canvas
  c->addLayer(t);
  const char faceName[] = "TIMES";
  titleFont = FontFace::get(faceName, 40, FontFace::BOLD);
  pageNumFont = FontFace::get(faceName, 30, FontFace::BOLD);
  normalFont = FontFace::get(faceName, 16, FontFace::BOLD);
  h1Font = FontFace::get(faceName, 24, FontFace::BOLD);
  h2Font = FontFace::get(faceName, 16, FontFace::BOLD);
  h3Font = FontFace::get(faceName, 14, FontFace::BOLD);
  rowSize = normalFont->getHeight();
}

// TODO: who is responsible for the Document being deallocated?
DocView::~DocView() {}

void DocView::init() {
  m->init();
  // img.init();
  t->init();
}

void DocView::process_input(Inputs* in, float dt) {}

inline void DocView::addChar(const Font* f, uint8_t c) {
  const PageLayout* layout = doc->getLayout();
  t->checkAdd(x, y, f, c, layout->x0, rowSize, layout->x1);
}

void DocView::printPageNum(uint32_t page) {
  const PageLayout* layout = doc->getLayout();
  t->add(layout->pageNumX, layout->pageNumY, pageNumFont, pageNum);
}

void DocView::update() {
  clear();
  const Page* p = doc->getPage(pageNum);
  const unsigned char* text = doc->getText();
  const PageLayout* layout = doc->getLayout();
  printPageNum(pageNum);
  x = layout->x0;
  y = layout->y0;
  const Font* f = style->f;

  uint32_t start = p->getLine(0);
  uint32_t next = p->getLine(1);
  for (uint32_t i = 1; i < p->size(); i++) {
    t->add(x, y, f, (const char*)(text+start), next-start+1);
#if 0
    for (uint32_t j = start; j < next; j++) {
      if (text[j] == ' ') {
        x += f->spaceWidth;
        if (x > layout->x1) {
          x = layout->x0;
          y += rowSize;
        }
      } else if (text[j] == '\n') {
        y += layout->paragraphSpacing;
        x = layout->x0;
      } else
        t->checkAdd(x, y, f, text[j], layout->x0, rowSize, layout->x1);
      //      cout << text[j]; // print
    }
#endif
    if (i < p->size() - 1) {
      start = next;
      next = p->getLine(i + 1);
      y += f->getHeight();
    }
  }
  //	GLWin::setDirty();
}

void DocView::render() {
  m->render();
  // img.render();
  t->render();
}

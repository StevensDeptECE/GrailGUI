#pragma once
#include <iostream>

#include "opengl/Document.hh"
#include "opengl/GLWin.hh"
#include "opengl/GLWinFonts.hh"
#include "opengl/Image.hh"
#include "opengl/MultiText.hh"
#include "opengl/Shape.hh"
#include "opengl/StyledMultiShape2D.hh"
class Style;
class Canvas;

class DocView : public Shape {
 private:
  const Document* doc;    // the underlying document
  StyledMultiShape2D* m;  // multishape for drawing 2d shapes in the background
  // Image img;            // image rendering for this document in front of
  // multishape
  MultiText* t;  // text rendering for this document on top of other two
  const Style* style;
  const Font* titleFont;
  const Font* pageNumFont;
  const Font* normalFont;
  const Font* h1Font;
  const Font* h2Font;
  const Font* h3Font;

  std::vector<float> vert;
  uint32_t pageNum;
  float x, y;
  float rowSize;  // the size of the current row. If there are different fonts,
                  // the size of the biggest + desired spacing

 public:
  DocView(Canvas* c, const Style* style, const Document* doc);
  ~DocView();
  void clear() {
    m->clear();
    // img.clear(); //clear all the images
    t->clear();  // clear all text
  }
  void printPageNum(uint32_t page);
  void print(float x, float y, const Font* f, uint32_t v);

  void advance() {
    if (pageNum < doc->getPageCount() - 1) pageNum++;
    update();
  }
  void advance10() {
    pageNum += 10;
    if (pageNum >= doc->getPageCount()) {
      pageNum = 0;
    }
    update();
  }

  void back() {
    if (pageNum > 0) pageNum--;
    update();
  }
  void top() {
    pageNum = 0;
    update();
  }
  void bottom() {
    pageNum = doc->getPageCount() - 1;
    update();
  }

  void gotoPage(uint32_t pageNum) {
    if (pageNum >= doc->getPageCount()) return;
    this->pageNum = pageNum;
  }

  void addChar(const Font* f, uint8_t c);
  void init();
  void process_input(Inputs* in, float dt);
  void update();
  void render();
};

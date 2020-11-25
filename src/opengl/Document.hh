#pragma once
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "opengl/GLWin.hh"
#include "util/HashMap.hh"

class PageLayout {
 public:
  float x0, x1;  // left and right margins
  float y0, y1;  // top and botton margins
  float w, h;    // width and height of the active part
  float pageNumX, pageNumY;
  float paragraphSpacing;
  uint32_t linesPerPage;
  const Font *f;  // normal font for this page. TODO: lots more detail later
  // TODO: list of content at standard locations?

  PageLayout(float x0, float y0, float w, float h, float pageNumX,
             float pageNumY, float paragraphSpacing, uint32_t linesPerPage,
             const Font *f)
      : x0(x0),
        y0(y0),
        x1(x0 + w),
        y1(y0 + h),
        w(w),
        h(h),
        pageNumX(pageNumX),
        pageNumY(pageNumY),
        paragraphSpacing(paragraphSpacing),
        linesPerPage(linesPerPage),
        f(f) {}
};

class Page {
 private:
  const unsigned char *text;
  const PageLayout *layout;
  std::vector<uint32_t> lines;

 public:
  Page(const unsigned char *text, const PageLayout *pageLayout,
       uint32_t offset);
  void addLine(uint32_t offset) { lines.push_back(offset); }
  uint32_t getLine(uint32_t line) const {
    if (line > size()) {
      std::cerr << "src/opengl/primitivesV2/"
                   "Document.hh:page::getLine(uint32_t): line out of bounds"
                << std::endl;
    }
    return lines[line];
  }
  const PageLayout *getLayout() const { return layout; }
  uint32_t size() const { return lines.size(); }
};


class Document {
 private:
  // TODO: the buffer should be rope
  unsigned char *text;  // the main buffer
  std::vector<Page>
      pages;  // TODO: Replace by rope for high performance editable document

  PageLayout layout;  // size of the page to render to
 public:
  Document(const PageLayout &layout) : layout(layout), text(nullptr) {}
  ~Document() { delete[] text; }
  Document(const Document &orig) = delete;
  Document &operator=(const Document &orig) = delete;

  const unsigned char *getText() const { return text; }
  const Page *getPage(uint32_t p) const { return &pages[p]; }
  const PageLayout *getLayout() const { return &layout; }
  uint32_t getPageCount() const { return pages.size(); }

  void appendFile(const PageLayout &layout, const char filename[]);
};

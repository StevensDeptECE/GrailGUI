#include "opengl/Document.hh"

#include <fstream>
#include <sstream>

#include "opengl/DocView.hh"
#include "opengl/GLWinFonts.hh"
#include "util/DynArray.hh"
using namespace std;

Page::Page(const unsigned char *text, const PageLayout *layout, uint32_t offset)
    : text(text), layout(layout) {
  // TODO: what to do with offset, store as local variable?
  lines.reserve(1024);
}

void Document::appendFile(const PageLayout &newLayout, const char filename[]) {
  ifstream t(filename);
  t.seekg(0, ios::end);
  uint32_t len = t.tellg();
  text = new unsigned char[len];
  t.seekg(0, std::ios::beg);
  t.read((char *)text, len);
  const Font *f = layout.f;
  pages.push_back(Page(text, &layout, 0));
  Page *page = &pages.back();
  page->addLine(0);
  // buildJustifiedText(layout, text, len);
  buildRaggedText(layout, text, len);
}

void Document::buildJustifiedText(const PageLayout &layout, unsigned char *text,
                                  uint32_t len) {
  float x = layout.x0, y = layout.y0;
  const Font *f = layout.f;
  float lineSize = f->getHeight();
  Page *page = &pages.back();
  unsigned char current;
  int j = 0;  // j compacts the document as it loads
  for (uint32_t i = 0; i < len; i++) {
    if (text[i] == '\n') {
      page->addLine(j);
      x = layout.x0;
      y += lineSize;
      continue;
    } else if (text[i] >= ' ') {  // every time a printable letter is found...
      current = text[j++] = text[i];  // copy it into the compacted list
    } else {
      continue;  // skip any character that is non-printable and ignore
    }

    /*
     text should now include no characters below space
     Each newline is now gone but the line structures point there
    */

    const Font::Glyph *glyph = f->getGlyph(current);
    x += glyph->advance;
    if (x > layout.x1) {
      x = layout.x0;
      y += lineSize;
      if (y >= layout.y1) {
        pages.push_back(Page(text, &layout, j));
        page = &pages.back();
        page->addLine(j);
        x = layout.x0 + glyph->advance;
        y = layout.y0;
      } else {
        page->addLine(j);
      }
    }
  }

  page->addLine(j);  // the new length is j
}

void Document::buildRaggedText(const PageLayout &layout, unsigned char *text,
                               uint32_t len) {
  float x = layout.x0, y = layout.y0;
  const Font *f = layout.f;
  float lineSize = f->getHeight();
  Page *page = &pages.back();
  unsigned char current;
  int j = 0;  // j compacts the document as it loads
  for (uint32_t i = 0; i < len; i++) {
    if (text[i] == '\n') {
      page->addLine(j);
      x = layout.x0;
      y += lineSize;
      continue;
    } else if (text[i] >= ' ') {  // every time a printable letter is found...
      current = text[j++] = text[i];  // copy it into the compacted list
    } else {
      continue;  // skip any character that is non-printable and ignore
    }

    /*
     text should now include no characters below space
     Each newline is now gone but the line structures point there
    */
    const Font::Glyph *glyph = f->getGlyph(current);
    x += glyph->advance;  // calculate right edge of this character to see
                          // whether to print

    if (x > layout.x1) {
      j--;  // undo the advance of j and see whether to prospectively print past
            // right margin
      float xtemp = x;
      for (uint32_t k = i + 1; k < len; k++) {
        if (text[k] == '\n' || text[k] == ' ' || text[k] == '-') {
          for (int m = 0; m < k - i; m++) text[j + m] = text[i + m];

          j += (k -
                i);  // move j forward to account for letters added to this line
          i = (text[k] != '-') ? k : k - 1;  // i advances to skip end of line
                                             // char, then at end of loop i++
          // but if - print on next line, not pretty but not skipping the -
          x = layout.x0;
          if (text[k] == '\n')
            y += lineSize;  // TODO: page layout should include amount to break
                            // paragraph
          // y += lineSize;
          page->addLine(j);
          goto nextLine;
        } else {
          glyph = f->getGlyph(text[k]);
          xtemp += glyph->advance;
          if (xtemp > layout.xDropDead) {
            text[j++] = text[i];
            break;
          }
        }
      }
      x = layout.x0;
      y += lineSize;
      if (y >= layout.y1) {
        pages.push_back(Page(text, &layout, j));
        page = &pages.back();
        page->addLine(j);
        x = layout.x0 + glyph->advance;
        y = layout.y0;
      } else {
        page->addLine(j);
      }
    }
  nextLine:;
  }
  page->addLine(j);  // the new length is j
}

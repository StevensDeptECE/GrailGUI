#include "opengl/MultiText2.hh"

#include <algorithm>
#include <numbers>

#include "glad/glad.h"
#include "opengl/Canvas.hh"
#include "opengl/GLWin.hh"
#include "opengl/GLWinFonts.hh"
#include "opengl/Shader.hh"
#include "opengl/Style.hh"

using namespace std;
// todo: fix render to pass everything in the text vert to draw it
// why init loop?
// once used to be a variable here.
/*
  Create a MultiText2 using the font and color in style.
  The size is the amount of text it can hold. It preallocates 24 floats per
  text to hold the coordinates for texturing
*/
MultiText2::MultiText2(Canvas* c, const Style* style, uint32_t numChars)
    : Shape(c), style(style), transform(1.0f), numChars(numChars) {
  // if !once, once = !once was a thing
  vert.reserve(numChars * 16);    // x,y,u,v (4 floats per character)
  indices.reserve(numChars * 5);  // 5 indicies per triangle strip i1 i2 i3 i4
                                  // and then 0xFFFFFFFF (terminator)
  const Font* f = style->f;       // FontFace::getFace(1)->getFont(0);
}

MultiText2::MultiText2(Canvas* c, const Style* style)
    : MultiText2(c, style, 16) {}

MultiText2::MultiText2(Canvas* c, const Style* style, uint32_t numChars,
                       float angle, float x, float y)
    : MultiText2(c, style, numChars) {
  transform = glm::translate(transform, glm::vec3(x, y, 0));
  transform = glm::rotate(transform, angle, glm::vec3(0, 0, -1));
  // transform = glm::translate(transform, glm::vec3(-x, -y, 0));
}

MultiText2::MultiText2(Canvas* c, const Style* style, float angle, float x,
                       float y)
    : MultiText2(c, style, 4096, angle, x, y) {}
MultiText2::~MultiText2() {}

// TODO: factor out code into addchar, make inline
//  will require an internal version that passes a glyph as parameter
//  but then there will be only one copy of this code.
void MultiText2::addChar(float x, float y, const Font* f, unsigned char c) {
  const Font::Glyph* glyph = f->getGlyph(c);
  cout << "Glyph for char " << c << "\n" << *glyph << '\n';
  float x0 = x + glyph->bearingX, x1 = x0 + glyph->sizeX;
  float y0 = y - glyph->bearingY, y1 = y0 + glyph->sizeY;
  // cout << "x=" << x << ", y=" << y << '\n';
  addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
  addPoint(x0, y1, /* fontLeft */ glyph->u0, glyph->v0);
  addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
  addPoint(x1, y0, /* fontRight */ glyph->u1, glyph->v1);

  x += glyph->advance;
}

inline void MultiText2::internalAdd(float x, float y, const Font* f,
                                    const char s[], uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    const Font::Glyph* glyph = f->getGlyph(s[i]);
    float x0 = x + glyph->bearingX, x1 = x0 + glyph->sizeX;
    // TODO: Not maxwidth, should be less for proportional fonts?
    float y0 = y - glyph->bearingY, y1 = y0 + glyph->sizeY;
    addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
    addPoint(x0, y1, /* fontLeft */ glyph->u0, glyph->v0);
    addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
    addPoint(x1, y0, /* fontRight */ glyph->u1, glyph->v1);

    x += glyph->advance;
  }
}
//TODO: Let's make unicode work!
// How does 19 bit unicode even work? hoping never to find out: https://home.unicode.org
#if 0
inline void MultiText2::internalAdd(float x, float y, const BigFont* f,
                                    const uint16_t s[], uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    const Font::Glyph* glyph = f->getGlyph(s[i]);
    float x0 = x + glyph->bearingX, x1 = x0 + glyph->sizeX;
    // TODO: Not maxwidth, should be less for proportional fonts?
    float y0 = y - glyph->bearingY, y1 = y0 + glyph->sizeY;
    addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
    addPoint(x0, y1, /* fontLeft */ glyph->u0, glyph->v0);
    addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
    addPoint(x1, y0, /* fontRight */ glyph->u1, glyph->v1);

    x += glyph->advance;
  }
}

inline void MultiText2::internalAdd(float x, float y, const BigFont* f,
                                    uint32_t baseCode, // the base code in unicode, each char=baseCode+s[i]
                                    const uint8_t s[], uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    const Font::Glyph* glyph = f->getGlyph(s[i]);
    float x0 = x + glyph->bearingX, x1 = x0 + glyph->sizeX;
    // TODO: Not maxwidth, should be less for proportional fonts?
    float y0 = y - glyph->bearingY, y1 = y0 + glyph->sizeY;
    addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
    addPoint(x0, y1, /* fontLeft */ glyph->u0, glyph->v0);
    addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
    addPoint(x1, y0, /* fontRight */ glyph->u1, glyph->v1);

    x += glyph->advance;
  }
}

#endif

inline void rotateAround(float xc, float yc, float cosa, float sina, float& x,
                         float& y) {
  const double dx = x - xc, dy = y - yc;
  x = xc + dx * cosa - dy * sina;
  y = yc + dx * sina + dy * cosa;
}

void MultiText2::add(float x, float y, float ang, const Font* f, const char s[],
                     uint32_t len) {
  float startx = x, starty = y;
  double cosa = cos(ang), sina = sin(ang);
  for (uint32_t i = 0; i < len; i++) {
    const Font::Glyph* glyph = f->getGlyph(s[i]);
    float x0 = x + glyph->bearingX,
          x1 = x0 + glyph->sizeX;  // TODO: Not maxwidth, should be less for
                                   // proportional fonts?
    float y0 = y - glyph->bearingY, y1 = y0 + glyph->sizeY;
    rotateAround(startx, starty, cosa, sina, x0, y0);
    rotateAround(startx, starty, cosa, sina, x1, y1);

    addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
    addPoint(x0, y1, /* fontLeft */ glyph->u0, glyph->v0);
    addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
    addPoint(x1, y0, /* fontRight */ glyph->u1, glyph->v1);

    x += glyph->advance;
  }
}

void MultiText2::add(float x, float y, uint32_t v) {
  char s[10];
  int len = sprintf(s, "%u", v);
  internalAdd(x, y, style->f, s, len);
}

void MultiText2::add(float x, float y, const Font* f, uint32_t v) {
  char s[10];
  int len = sprintf(s, "%u", v);
  internalAdd(x, y, f, s, len);
}

void MultiText2::add(float x, float y, const Font* f, int32_t v) {
  char s[10];
  int len = sprintf(s, "%d", v);
  internalAdd(x, y, f, s, len);
}

void MultiText2::addHex(float x, float y, const Font* f, uint32_t v) {
  char s[10];
  int len = sprintf(s, "%x", v);
  internalAdd(x, y, f, s, len);
}
void MultiText2::addHex8(float x, float y, const Font* f, uint32_t v) {
  char s[10];
  int len = sprintf(s, "%08x", v);
  internalAdd(x, y, f, s, len);
}

void MultiText2::add(float x, float y, float v) {
  char s[20];
  int len = sprintf(s, "%f", v);
  internalAdd(x, y, style->f, s, len);
}

void MultiText2::add(float x, float y, const Font* f, float v) {
  char s[20];
  int len = sprintf(s, "%f", v);
  internalAdd(x, y, f, s, len);
}

void MultiText2::add(float x, float y, double v) {
  char s[25];
  int len = sprintf(s, "%4.4lf", v);
  internalAdd(x, y, style->f, s, len);
}

void MultiText2::add(float x, float y, const Font* f, double v) {
  char s[25];
  int len = sprintf(s, "%4.4lf", v);
  internalAdd(x, y, f, s, len);
}

void MultiText2::add(float x, float y, const Font* f, double v, int fieldWidth,
                     int precision) {
  char fmt[35];
  sprintf(fmt, "%%%d.%dlf", fieldWidth, precision);
  char s[35];
  int len = sprintf(s, fmt, v);
  internalAdd(x, y, f, s, len);
}

void MultiText2::addCentered(float x, float y, const Font* f, double v,
                             int fieldWidth, int precision) {
  char fmt[35];
  sprintf(fmt, "%%%d.%dlf", fieldWidth, precision);
  char s[35];
  int len = sprintf(s, fmt, v);

  float textWidth = f->getWidth(s, len);
  float textHeight = f->getHeight();

  internalAdd(x - textWidth / 2, y - textHeight / 2, f, s, len);
}

void MultiText2::addCentered(float x, float y, const Font* f, const char s[],
                             uint32_t len) {
  float textWidth = f->getWidth(s, len);
  float textHeight = f->getHeight();

  internalAdd(x - textWidth / 2, y - textHeight / 2, f, s, len);
}

void MultiText2::add(float x, float y, const Font* f, const char s[],
                     uint32_t len) {
  internalAdd(x, y, f, s, len);
}

void MultiText2::add(float x, float y, const Font* f, const std::string& s) {
  internalAdd(x, y, f, s.c_str(), s.length());
}

/*
  Draw a string of fixed length with the default font in the style of the
  MultiText
*/
void MultiText2::add(float x, float y, const char s[], uint32_t len) {
  internalAdd(x, y, style->f, s, len);
}

// find the index of the first character over the margin with this font
uint32_t MultiText2::findFirstOverMargin(float x, const Font* f, const char s[],
                                         uint32_t len, float rightMargin) {
  uint32_t i = 0;
  while (true) {
    const Font::Glyph* g = f->getGlyph(s[i]);
    if (x + g->advance < rightMargin) {
      x += g->advance;
      i++;
    } else {
      if (x + g->bearingX + g->sizeX > rightMargin) return i;
      return i + 1;
    }
  }
}

void MultiText2::checkAdd(float& x, float& y, const Font* f,
                          const unsigned char c, float leftMargin,
                          float rowSize, float rightMargin) {
  const Font::Glyph* g = f->getGlyph(c);
  if (x + g->bearingX + g->sizeX > rightMargin) {
    x = leftMargin;  // really left margin for now, but perhaps later...
    y += rowSize;
  }
  add(x, y, f, c);
}

const Style* MultiText2::getStyle() { return style; }

void MultiText2::update() {}

void MultiText2::init() {
  //  cout << "multitext init" <<endl;
  textureId = style->f->getTexture();

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vert.capacity(), (void*)0,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4,
                        (void*)(sizeof(float) * 2));

  // Create the indicies that draw each glyph. Each is a a triangle_strip so 4
  // indicies j, j+1, j+2, j+3 draws a rectangle and then 0xFFFFFFFF starts a
  // new glyph
  for (uint32_t i = 0, j = 0; i < numChars; i += 5, j += 4) {
    indices[i] = j;
    indices[i + 1] = j + 1;
    indices[i + 2] = j + 2;
    indices[i + 3] = j + 3;
    indices[i + 4] = 0xFFFFFFFFU;
  }
  glGenBuffers(1, &sbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.capacity(),
               &indices[0], GL_DYNAMIC_DRAW);
  // Unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

static uint32_t pow10(uint32_t v) {
  int lead = __builtin_clzll(v);
  return lead / 3;
}
static uint32_t pow10arr[10] = {
    0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

void MultiText2::render(glm::mat4& trans) {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  Shader* s = Shader::useShader(GLWin::TEXT_SHADER);
  s->setVec4("textColor", style->getFgColor());
  s->setMat4("projection", *parentCanvas->getProjection() * transform);
  s->setInt("ourTexture", 0);

  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(0xFFFFFFFFU);

  // glPushAttrib(GL_CURRENT_BIT);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);

  // Update points for any new chars
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, vert.size() * sizeof(float), &vert[0]);

  // Draw characters using 4 points to glyph (triangle strip) terminated by
  // special value 0xFFFFFFFF
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lbo);
  glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glDisable(GL_PRIMITIVE_RESTART);
}

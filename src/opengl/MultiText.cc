#include "opengl/MultiText.hh"

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
  Create a MultiText using the font and color in style.
  The size is the amount of text it can hold. It preallocates 24 floats per
  text to hold the coordinates for texturing
*/
MultiText::MultiText(Canvas* c, const Style* style, uint32_t size)
    : Shape(c), style(style), transform(1.0f) {
  // if !once, once = !once was a thing
  vert.reserve(size * 24);
  const Font* f = style->f;  // FontFace::getFace(1)->getFont(0);
}

MultiText::MultiText(Canvas* c, const Style* style) : MultiText(c, style, 16) {}

MultiText::MultiText(Canvas* c, const Style* style, uint32_t size, float angle,
                     float x, float y)
    : MultiText(c, style, size) {
  transform = glm::translate(transform, glm::vec3(x, y, 0));
  transform = glm::rotate(transform, angle, glm::vec3(0, 0, -1));
  // transform = glm::translate(transform, glm::vec3(-x, -y, 0));
}

MultiText::MultiText(Canvas* c, const Style* style, float angle, float x,
                     float y)
    : MultiText(c, style, 16, angle, x, y) {}

MultiText::~MultiText() {}

void MultiText::init() {
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

void MultiText::addChar(float x, float y, const Font* f, unsigned char c) {
  const Font::Glyph* glyph = f->getGlyph(c);
  cout << "Glyph for char " << c << "\n" << *glyph << '\n';
  float x0 = x + glyph->bearingX, x1 = x0 + glyph->sizeX;
  float y0 = y - glyph->bearingY, y1 = y0 + glyph->sizeY;
  // cout << "x=" << x << ", y=" << y << '\n';
  addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
  addPoint(x0, y1, /* fontLeft */ glyph->u0, glyph->v0);
  addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
  addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
  addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
  addPoint(x1, y0, /* fontRight */ glyph->u1, glyph->v1);

  x += glyph->advance;
}

inline float MultiText::internalAdd(float x, float y, const Font* f,
                                    const char s[], uint32_t len) {
  for (uint32_t i = 0; i < len; i++) {
    const Font::Glyph* glyph = f->getGlyph(s[i]);
    float x0 = x + glyph->bearingX,
          x1 = x0 + glyph->sizeX;  // TODO: Not maxwidth, should be less for
                                   // proportional fonts?
    float y0 = y - glyph->bearingY, y1 = y0 + glyph->sizeY;
    addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
    addPoint(x0, y1, /* fontLeft */ glyph->u0, glyph->v0);
    addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
    addPoint(x0, y0, /* fontLeft */ glyph->u0, glyph->v1);
    addPoint(x1, y1, /* fontRight */ glyph->u1, glyph->v0);
    addPoint(x1, y0, /* fontRight */ glyph->u1, glyph->v1);

    x += glyph->advance;
  }
  return x;
}

// #if 0
// // void MultiText::add(float x, float y, uint32_t v) {
// //   const Font* f = style->f;
// // 	int lead = __builtin_clzll(v);
// //   if(lead == 0) {
// // 		addChar(f, v + '0');
// // 		return;
// // 	}
// // 	uint32_t whichp10 = lead / 3;
// // 	uint32_t pow = pow10[whichp10];
// // 	cout << lead << ": " << pow << '\n';
// // 	uint32_t hi = v / pow, lo = v % pow;
// // 	if (hi >= 10) {
// // 		addChar(f, hi/10 + '0');
// // 		addChar(f, hi%10 + '0');
// // 	} else {
// // 		addChar(f, hi + '0');
// // 	}
// // 	for ( ; whichp10 >= 4; whichp10 -= 4) {
// // 		uint32_t nextPow = pow10[whichp10-4];
// // 		hi = low / nextPow;
// // 		low = low - hi * nextPow;
// // 		uint32_t digits = digits[hi];
// // 		addChar(f, digits >> 24);
// // 		addChar(f, (digits >> 16) & 0xFF);
// // 		addChar(f, (digits >> 8) & 0xFF);
// // 		addChar(f, (digits >> 16) & 0xFF);
// // 	}
// // }
// #endif
float MultiText::add(float x, float y, uint32_t v) {
  char s[10];
  int len = sprintf(s, "%u", v);
  return internalAdd(x, y, style->f, s, len);
}

float MultiText::add(float x, float y, const Font* f, uint32_t v) {
  char s[10];
  int len = sprintf(s, "%u", v);
  return internalAdd(x, y, f, s, len);
}

float MultiText::add(float x, float y, const Font* f, int32_t v) {
  char s[10];
  int len = sprintf(s, "%d", v);
  return internalAdd(x, y, f, s, len);
}

float MultiText::addHex(float x, float y, const Font* f, uint32_t v) {
  char s[10];
  int len = sprintf(s, "%x", v);
  return internalAdd(x, y, f, s, len);
}
float MultiText::addHex8(float x, float y, const Font* f, uint32_t v) {
  char s[10];
  int len = sprintf(s, "%08x", v);
  return internalAdd(x, y, f, s, len);
}

float MultiText::add(float x, float y, float v) {
  char s[20];
  int len = sprintf(s, "%f", v);
  return internalAdd(x, y, style->f, s, len);
}

float MultiText::add(float x, float y, const Font* f, float v) {
  char s[20];
  int len = sprintf(s, "%f", v);
  return internalAdd(x, y, f, s, len);
}

float MultiText::add(float x, float y, double v) {
  char s[25];
  int len = sprintf(s, "%4.4lf", v);
  return internalAdd(x, y, style->f, s, len);
}

float MultiText::add(float x, float y, const Font* f, double v) {
  char s[25];
  int len = sprintf(s, "%4.4lf", v);
  return internalAdd(x, y, f, s, len);
}

float MultiText::add(float x, float y, const Font* f, double v, int fieldWidth,
                    int precision) {
  char fmt[35];
  sprintf(fmt, "%%%d.%dlf", fieldWidth, precision);
  char s[35];
  int len = sprintf(s, fmt, v);
  return internalAdd(x, y, f, s, len);
}

void MultiText::addCentered(float x, float y, const Font* f, double v,
                            int fieldWidth, int precision) {
  char fmt[35];
  sprintf(fmt, "%%%d.%dlf", fieldWidth, precision);
  char s[35];
  int len = sprintf(s, fmt, v);

  float textWidth = f->getWidth(s, len);
  float textHeight = f->getHeight();

  internalAdd(x - textWidth / 2, y - textHeight / 2, f, s, len);
}

// Horizontally centered text
void MultiText::addCentered(float x, float y, float w, const Font* f,
                            const char s[], uint32_t len) {
  float textWidth = f->getWidth(s, len);

  internalAdd(x + (w - textWidth) / 2, y, f, s, len);
}

// Horizontally centered text
void MultiText::addCentered(float x, float y, float w, const Font* f,
                            const string& s) {
  addCentered(x, y, w, f, s.c_str(), s.length());
}

// Text centered on x,y (horizontal and vertical)
void MultiText::addCentered(float x, float y, const Font* f, const char s[],
                            uint32_t len) {
  float textWidth = f->getWidth(s, len);
  float textHeight = f->getHeight();

  internalAdd(x - textWidth / 2, y - textHeight / 2, f, s, len);
}

// horizontally and vertically centered text
void MultiText::addCentered(float x, float y, float w, float h, const Font* f,
                            const char s[], uint32_t len) {
  float textWidth = f->getWidth(s, len);
  float textHeight = f->getHeight();

  internalAdd(x - textWidth / 2, y - textHeight / 2, f, s, len);
}

inline void rotateAround(float xc, float yc, float cosa, float sina, float& x,
                         float& y) {
  const double dx = x - xc, dy = y - yc;
  x = xc + dx * cosa - dy * sina;
  y = yc + dx * sina + dy * cosa;
}
void MultiText::add(float x, float y, float space, float ang, const Font* f,
                    const char s[], uint32_t len) {
  float startx = x, starty = y;
  x += space;
  double cosa = cos(ang), sina = sin(ang);
  // glm::mat4 t(1);
  // t = glm::translate(t, glm::vec3(-x,-y,0));
  // t = glm::rotate(t, ang, glm::vec3(0,0,1));
  // t = glm::translate(t, glm::vec3(x,y,0.0f));
  for (uint32_t i = 0; i < len; i++) {
    const Font::Glyph* glyph = f->getGlyph(s[i]);
    float x0 = x + glyph->bearingX,
          x1 = x0 + glyph->sizeX;  // TODO: Not maxwidth, should be less for
                                   // proportional fonts?
    float y0 = y - glyph->bearingY, y1 = y0 + glyph->sizeY;
    ;
    // glm::vec4 vec0(x0, y0, 0, 1);
    // vec0 = t*vec0;
    // x0 = vec0.x, y0 = vec0.y;
    // glm::vec4 vec1(x1, y1, 0, 1);
    // vec1 = t*vec1;
    // x1 = vec1.x, y1 = vec1.y;

    float xt = x0, yt = y0;
    rotateAround(startx, starty, cosa, sina, xt, yt);
    addPoint(xt, yt, /* fontLeft */ glyph->u0, glyph->v1);

    xt = x0, yt = y1;
    rotateAround(startx, starty, cosa, sina, xt, yt);
    addPoint(xt, yt, /* fontLeft */ glyph->u0, glyph->v0);

    xt = x1, yt = y1;
    rotateAround(startx, starty, cosa, sina, xt, yt);
    addPoint(xt, yt, /* fontRight */ glyph->u1, glyph->v0);

    xt = x0, yt = y0;
    rotateAround(startx, starty, cosa, sina, xt, yt);
    addPoint(xt, yt, /* fontLeft */ glyph->u0, glyph->v1);

    xt = x1, yt = y1;
    rotateAround(startx, starty, cosa, sina, xt, yt);
    addPoint(xt, yt, /* fontLeft */ glyph->u0, glyph->v1);

    xt = x1, yt = y0;
    rotateAround(startx, starty, cosa, sina, xt, yt);
    addPoint(xt, yt, /* fontRight */ glyph->u1, glyph->v1);

    x += glyph->advance;
  }
  return x;
}

void MultiText::add(float x, float y, const Font* f, const char s[],
                    uint32_t len) {
  internalAdd(x, y, f, s, len);
}

void MultiText::add(float x, float y, const Font* f, const std::string& s) {
  internalAdd(x, y, f, s.c_str(), s.length());
}

/*
  Draw a string of fixed length with the default font in the style of the
  MultiText
*/
void MultiText::add(float x, float y, const char s[], uint32_t len) {
  internalAdd(x, y, style->f, s, len);
}

// find the index of the first character over the margin with this font
uint32_t MultiText::findFirstOverMargin(float x, const Font* f, const char s[],
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

void MultiText::checkAdd(float& x, float& y, const Font* f,
                         const unsigned char c, float leftMargin, float rowSize,
                         float rightMargin) {
  const Font::Glyph* g = f->getGlyph(c);
  if (x + g->bearingX + g->sizeX > rightMargin) {
    x = leftMargin;  // really left margin for now, but perhaps later...
    y += rowSize;
  }
  add(x, y, f, c);
}

const Style* MultiText::getStyle() { return style; }

void MultiText::update() {}

void MultiText::render() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  Shader* s = Shader::useShader(GLWin::TEXT_SHADER);
  s->setVec4("textColor", style->getFgColor());
  s->setMat4("projection", *parentCanvas->getProjection() * transform);
  s->setInt("ourTexture", 0);

  // glPushAttrib(GL_CURRENT_BIT);
  // glColor3f(0, 0, 255);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureId);

  // glPopAttrib();
  // Update data
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  const int windowLen = 128 * 128 * 24;  // preallocate a
  46 * (2 + 3 + 6 + 4) *
      24;  // TODO: This was too small. We need a better policy?
  glBufferSubData(GL_ARRAY_BUFFER, 0, vert.size() * sizeof(float),
                  &vert[0]);  // TODO: Right now we draw the entire string!
  glDrawArrays(GL_TRIANGLES, 0, vert.size());

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

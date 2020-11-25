#include "opengl/MultiText.hh"
#include "opengl/Style.hh"
#include "opengl/GLWin.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include <algorithm>

using namespace std;

PageText::PageText(const Style * style, uint32_t size, float right, float bottom) :
  Shape(), style(style), right(right), bottom(bottom) {
  vert.reserve(size * 24);
  const Font* f = style->f;//FontFace::getFace(1)->getFont(0);
}

void PageText::init(){
  textureId = style->f->getTexture();
  
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);

  glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vert.size(),(void*)0,GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(void*)0);
  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(void*)(sizeof(float)*2));

  //Unbind
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);
}

static uint32_t pow10(uint32_t v) {
	int lead = __builtin_clzll(v);
	return lead / 3;
}
static uint32_t pow10arr[10] = {
														 0, 10, 100,1000,
														 10000, 100000, 1000000, 10000000,
														 100000000, 1000000000 };

inline void PageText::addChar(float left, float top, Font* f, char c) {
	const Font::Glyph* glyph = f->getGlyph(c);
	const float fontLeft = 0.0;
	const float fontRight = 1.0;
  float x = left, y = top;
  float x0, x1, y0, y1;

  x0 = x+glyph->bearing.x, x1 = x0 + f->maxWidth; // TODO: Not maxwidth, should be less for proportional fonts?
  y0 = y-glyph->bearing.y, y1 = y0 + glyph->size.y;
  if ( x1 > right) {
    x0 = left, x1 = x0 + f->maxWidth;
    y += f->height;
    y0 = y-glyph->bearing.y, y1 = y0 + glyph->size.y;
  }    
	addPoint(x0, y0, fontLeft, glyph->v1);
	addPoint(x0, y1, fontLeft, glyph->v0);
	addPoint(x1, y1, fontRight, glyph->v0);
	addPoint(x0, y0, fontLeft, glyph->v1);
	addPoint(x1, y1, fontRight, glyph->v0);
	addPoint(x1, y0, fontRight, glyph->v1);

	x += glyph->advance;
}

#if 0
void MultiText::add(float x, float y, uint32_t v) {
  const Font* f = style->f;
	int lead = __builtin_clzll(v);
	if(lead == 0) {
		addChar(f, v + '0');
		return;
	}
	uint32_t whichp10 = lead / 3;
	uint32_t pow = pow10[whichp10];
	cout << lead << ": " << pow << '\n';
	uint32_t hi = v / pow, lo = v % pow;
	if (hi >= 10) {
		addChar(f, hi/10 + '0');
		addChar(f, hi%10 + '0');
	} else {
		addChar(f, hi + '0');
	}
	for ( ; whichp10 >= 4; whichp10 -= 4) {
		uint32_t nextPow = pow10[whichp10-4];
		hi = low / nextPow;
		low = low - hi * nextPow;
		uint32_t digits = digits[hi];
		addChar(f, digits >> 24);
		addChar(f, (digits >> 16) & 0xFF);
		addChar(f, (digits >> 8) & 0xFF);
		addChar(f, (digits >> 16) & 0xFF);
	}
}
#endif
void PageText::add(float x, float y, uint32_t v) {
	char s[10];
	int len = sprintf(s, "%u", v);
	add(x, y, s, len);
}

void PageText::add(float x, float y, float v) {
	char s[20];
	int len = sprintf(s, "%f", v);
	add(x, y, s, len);
}

void MultiText::add(float x, float y, double v) {
	char s[25];
	int len = sprintf(s, "%4.4lf", v);
	add(x, y, s, len);
}

void PageText::add(float x, float y, const char s[], uint32_t len) {
  const float fontLeft = 0.0;  //TODO: This has to be wrong for multiple fonts per texture?
  const float fontRight = 1.0; //TODO: should be whatever fraction the letter is?
  const Font* f = style->f;
  for (uint32_t i = 0; i < len; i++) {
    const Font::Glyph* glyph = f->getGlyph(s[i]);
    float x0 = x+glyph->bearing.x, x1 = x0 + f->maxWidth; // TODO: Not maxwidth, should be less for proportional fonts?
    float y0 = y-glyph->bearing.y, y1 = y0 + glyph->size.y;
    if (x1 >= right
    addPoint(x0, y0, fontLeft, glyph->v1);
    addPoint(x0, y1, fontLeft, glyph->v0);
    addPoint(x1, y1, fontRight, glyph->v0);
    addPoint(x0, y0, fontLeft, glyph->v1);
    addPoint(x1, y1, fontRight, glyph->v0);
    addPoint(x1, y0, fontRight, glyph->v1);

    x += glyph->advance;
  }

}

void PageText::update()
{}

void PageText::render(){
  glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  Shader* s = Shader::useShader(GLWin::TEXT_SHADER);
  s->setVec4("textColor",style->getFgColor());
  s->setMat4("projection",*(parentCanvas->getProjection()));
  s->setInt("ourTexture",0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,textureId);

  //Update data
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
	const int windowLen = 46 * (2 + 3 + 6 + 4) * 24;
	//	cout << vert.size() << '\n';
  glBufferSubData(GL_ARRAY_BUFFER,0,windowLen*sizeof(float),&vert[0]);
  glDrawArrays(GL_TRIANGLES,0,vert.size());

  glDisableVertexAttribArray(1); 
  glDisableVertexAttribArray(0); 
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);
}

#include "opengl/Text.hh"
#include "opengl/Style.hh"
#include "opengl/GLWin.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "grail/Grail.hh"
#include <algorithm>

using namespace std;

void Text::init(){
  f = style->f;//FontFace::getFace(1)->getFont(0);
  textureId = f->getTexture();
  vert.reserve(LINE_LENGTH*24);

  #if 0
  for (int i = 0; i < LINE_LENGTH; i++) {
    addPoint(x, y, 0.0, 1.0);//x0
    addPoint(x, y, 0.0, 0.0);//x0
    addPoint(x, y, 1.0, 0.0);//x1
    addPoint(x, y, 0.0, 1.0);//x0
    addPoint(x, y, 1.0, 0.0);//x1
    addPoint(x, y, 1.0, 1.0);//x1
  }
  #endif

  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);

  glBufferData(GL_ARRAY_BUFFER,sizeof(float)*LINE_LENGTH*6*4,(void*)0,GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(void*)0);
  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(void*)(sizeof(float)*2));

  //Unbind
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);
}
int countRender = 0;

Text::~Text() {}
void Text::update()
{}

void Text::render(){
  // Iterate through all characters
  uint32_t end = std::min(LINE_LENGTH, uint32_t(text.length()));
  float xPos = x;
  float yPos = y;
  for (int i = 0, j = 0; j < end; i += 24, j++) {
    uint8_t c = text[j];
    if(c == 10){
      yPos+=style->f->getHeight();
      xPos=x;
      continue;
    }
    const Font::Glyph* glyph = f->getGlyph(c);
    #if 0 
    vert[i] =xPos+glyph->bearing.x; vert[i+1] = yPos-glyph->bearing.y; vert[i+3] = glyph->v1;
    vert[i+4]  =xPos+glyph->bearing.x; vert[i+5] = yPos-glyph->bearing.y+glyph->size.y; vert[i+7]  = glyph->v0;
    vert[i+8]  =xPos+glyph->bearing.x+f->maxWidth; vert[i+9] = yPos-glyph->bearing.y+glyph->size.y; vert[i+11] = glyph->v0;

    vert[i+12] =xPos+glyph->bearing.x; vert[i+13]=yPos-glyph->bearing.y; vert[i+15] = glyph->v1;
    vert[i+16] =xPos+glyph->bearing.x+f->maxWidth; vert[i+17] = yPos-glyph->bearing.y+glyph->size.y; vert[i+19] = glyph->v0;
    vert[i+20] =xPos+glyph->bearing.x+f->maxWidth;vert[i+21] = yPos-glyph->bearing.y; vert[i+23] = glyph->v1;
    #endif

    vert[i] = (xPos+glyph->bearingX); vert[i+1] = (yPos-glyph->bearingY);
    vert[i+2] = (glyph->u0); vert[i+3] = (glyph->v1);
    vert[i+4] = (xPos+glyph->bearingX); vert[i+5] = (yPos-glyph->bearingY + glyph->sizeY);
    vert[i+6] = (glyph->u0); vert[i+7] = (glyph->v0);
    vert[i+8] = (xPos+glyph->bearingX+f->maxWidth); vert[i+9] = (yPos-glyph->bearingY+glyph->sizeY);
    vert[i+10] = (glyph->u1); vert[i+11] = (glyph->v0);

    vert[i+12] = (xPos+glyph->bearingX); vert[i+13] = (yPos-glyph->bearingY);
    vert[i+14] = (glyph->u0); vert[i+15] = (glyph->v1);
    vert[i+16] = (xPos+glyph->bearingX+f->maxWidth); vert[i+17] = (yPos-glyph->bearingY+glyph->sizeY);
    vert[i+18] = (glyph->u1); vert[i+19] = (glyph->v0);
    vert[i+20] = (xPos+glyph->bearingX+f->maxWidth); vert[i+21] = (yPos-glyph->bearingY); //switched
    vert[i+22] = (glyph->u1); vert[i+23] = (glyph->v1);

    xPos += glyph->advance;
  }
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  Shader* s = Shader::useShader(Grail::TEXT_SHADER);
  s->setVec4("textColor",style->getFgColor());
  //cout << "Text color=" <<
  //  style->getFgColor().r << ',' << style->getFgColor().g << ',' << style->getFgColor().b << '\n';

  s->setMat4("projection",*(parentCanvas->getProjection()));
  s->setInt("ourTexture",0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,textureId);

  //Update data
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glBufferSubData(GL_ARRAY_BUFFER,0,end*24*sizeof(float),&vert[0]);
  glDrawArrays(GL_TRIANGLES,0,end*6);

  glDisableVertexAttribArray(1); 
  glDisableVertexAttribArray(0); 
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);

}
/*
OLD LOOP, Had size.x instead of maxSize"
  for (int i = 0, j = 0; j < end; i += 24, j++) {
    uint8_t c = text[j];
    const Font::Glyph* glyph = f->getGlyph(c);
    vert[i] =xPos+glyph->bearing.x; vert[i+1] = y-glyph->bearing.y; vert[i+3] = glyph->v1;
    vert[i+4]  =xPos+glyph->bearing.x; vert[i+5] = y-glyph->bearing.y+glyph->size.y; vert[i+7]  = glyph->v0;
    vert[i+8]  =xPos+glyph->bearing.x+glyph->size.x; vert[i+9] = y-glyph->bearing.y+glyph->size.y; vert[i+11] = glyph->v0;

    vert[i+12] =xPos+glyph->bearing.x; vert[i+13]=y-glyph->bearing.y; vert[i+15] = glyph->v1;
    vert[i+16] =xPos+glyph->bearing.x+glyph->size.x; vert[i+17] = y-glyph->bearing.y+glyph->size.y; vert[i+19] = glyph->v0;
    vert[i+20] =xPos+glyph->bearing.x+glyph->size.x;vert[i+21] = y-glyph->bearing.y; vert[i+23] = glyph->v1;
    xPos += glyph->advance;
  }*/

  const uint32_t Text::LINE_LENGTH;

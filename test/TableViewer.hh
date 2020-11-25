#pragma once

#include "opengl/Shape.hh"
#include "opengl/GLWinFonts.hh"
#include "Person.hh"
#include "util/DynArray.hh"
#include <cstdlib>
#include "glad/glad.h"

class TableViewer : public StyleMultiShape {
private:
  float x0,y0; //top left corner of the window viewed within the table
	float x,y; // current position
	const BigStyle* tableStyle;
  DynArray<BigStyle*> columnStyles;
  DynArray<uint32_t> columnIndex; //TODO: add ability to reorder columns
  DynArray<BigStyle*> rowStyles;
	const DynArray<Person> rows;
  DynArray<Text*> cells;
  uint32_t textureId;

public:
  TableViewer(const std::vector<Person>& list, float x0, float y0)
		: Shape(), list(&list), x0(x0),y0(y0){}

	float addChar(const char v, float x, float y) {
		const Font::Glyph* glyph = f->getGlyph(v);
		const float gx = glyph->bearing.x, gy = glyph->bearing.y;
		const float sizex = glyph->size.x, sizey = glyph->size.y;
		addPoint(x+gx, y-gy, 0, glyph->v1);
		addPoint(x+gx, y-gy+sizey, 0, glyph->v0);
		addPoint(x+gx+sizex, y-gy+sizey, 1.0, glyph->v0); 
		addPoint(x+gx, y-gy, 0.0, glyph->v1);
		addPoint(x+gx+sizex, y-gy+sizey, 1.0, glyph->v0);
		addPoint(x+gx+sizex, y-gy, 1.0, glyph->v1);
		x += glyph->advance;
		return x;
	}
	
#if 0
	void addField(const std::string& v) {
		for (int i = 0; i < v.length(); i++)
			x += addChar(v[i], x, y);
	}

	void addField(uint32_t v) {
		char buf[12];
		char* p = itoa((int)v, &buf[0], 10);
		for (int i = 0; p[i] != '\0'; i++)
			x += addChar(p[i], x, y);
	}
  template<typename T2>
  void addField(DataType typeCode, Format fmt, T2 Person::*ptr) {
  }

  //todo: support insert arbitrary column order???
  
  void addRow(Person p) {
    rows.add(p);
    addField(DataType::UINT64, "%ld", &Person::id);
    addField(DataType::STRING8, "%s", &Person::lastName);
    addField(DataType::STRING8, "%s", &Person::firstName);
    addField(DataType::STRING8, "%s", &Person::firstName);
  }

  
#endif
  void addRow(Person p) {
      
  }
    
  void init();
  void render(); 
};



void TableViewer::init() {
  textureId = f->getTexture();
  vert.reserve(list->size() * 4 *24);
	x = x0, y = y0;
 
	const int dy = f->getHeight();
  for (int i = 0; i < list->size(); i++, y += dy) {		
		addField((*list)[i].firstName); x += 100;
		addField((*list)[i].lastName); x += 100;
		addField((*list)[i].room); x += 100;
		addField((*list)[i].phone); x += 100;
		addField((*list)[i].email);
		x = x0;
	}

  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);

	//TODO: probably not *6*4! no time to check
  glBufferData(GL_ARRAY_BUFFER,sizeof(float)*vert.size()*6*4,&vert[0],GL_STATIC_DRAW);

  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(void*)0);
  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(float)*4,(void*)(sizeof(float)*2));

  //Unbind
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);

}

void TableViewer::render() {
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

	Style *style = new Style(); // TODO: make a real style!
  Shader* s = Shader::useShader(CompiledWebWindow::TEXT_SHADER);
  s->setVec3("textColor", style->fg);
  s->setMat4("projection",*(GLWin::getProjection()));
  s->setInt("ourTexture",0);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,textureId);

  //Update data
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glDrawArrays(GL_TRIANGLES,0,vert.size);

  glDisableVertexAttribArray(1); 
  glDisableVertexAttribArray(0); 
  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);
}

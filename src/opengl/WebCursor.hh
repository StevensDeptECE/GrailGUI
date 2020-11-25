#pragma once

#include "opengl/Canvas.hh"
#include "opengl/MultiShape2D.hh"
#include "opengl/MultiText.hh"
#include "opengl/GLWinFonts.hh"

#include <iostream>

using namespace std;


/*
  WebCursor represents the current location within a Shape, which  can be updated
  in order to stream visual objects into a rectangular region
 */
class WebCursor {
private:
  Canvas* c;
	//StyledMultiShape2D* s;
    MultiShape2D* s; 
    MultiText* m;
	float x, y;
  float width, height;
public:

  WebCursor(Canvas* c, MultiShape2D* s) : 
    c(c), s(s)
  {
      // start at the top left corner
      x = 0.0f;
      y = 0.0f;

      // get the width and height in pixels
      // from the associated Shape's parent
      // Canvas. These represent the bounds for the
      // WebCursor.
      width = c->getWidth();
      height = c->getHeight();

      cout << "Canvas dimensions: (" << width << ", " << height << ")\n";

  }

  WebCursor(Canvas* c, MultiText* m) : 
    c(c), m(m)
  {
      x = 0.0f;
      y = 0.0f;

      width = c->getWidth();
      height = c->getHeight();

      cout << "Canvas dimensions: (" << width << ", " << height << ")\n";

  }

  void addText(Style* st, const char text[], uint32_t len) {
    //for (size_t i = 0; i < len; i++) {
      //const Glyph* gly = s->f->getGlyph(text[i]);
      //cout << "advance:" << gly->advance << "\n";

      // draw the glyph
      //Multi
      //Text t = t->addText(x, y, st, text, len);
      //t.init();
      //t.render();


      // advance cursor
      //moveRight(gly->advance, s->f->getHeight());
    //}
  };

	void moveRight(float w, float h) {
		if (x + w > width) {
			x = 0;
			y += h;
		} else
			x += w;
	}
	void moveLeft(float w, float h) {
		if (x < w) {
			x = width - 1;
			y += h;
		} else
			x -= w;
	}

};

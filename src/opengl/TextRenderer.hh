#pragma once

#include "opengl/Shape2D.hh"
#include "opengl/shader.hpp"
#include <string>
#include <vector>
#include "opengl/GLWinFonts.hh"

class GLWin;

class Text : public Shape2D {
private:
	GLWin* w;
	const Font* f;
	uint32_t textureId;
	float x0, y0;
	std::string text;
	std::vector<float> vert;
	void addPoint(float x, float y, float u, float v) {
		vert.push_back(x); vert.push_back(y);
		vert.push_back(u); vert.push_back(v);
	}
public:
  Text(GLWin* w, float x, float y, const std::string& text) :
		Shape2D(x, y), w(w), x0(x), y0(y), text(text) {}
	constexpr static uint32_t LINE_LENGTH = 256;
	void init();
  void render();
	
};

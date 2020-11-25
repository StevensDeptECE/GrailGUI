#pragma once

#include "opengl/Shape.hh"
#include <vector>


class ColorGrid : public Shape {
private:
	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	std::vector<float> colors;
	uint32_t numPoints;
public:
  ColorGrid(uint32_t xsize, uint32_t ysize) {
		numPoints = xsize * ysize;
		uint32_t triCount = (xsize-1)*(ysize-1)*2;
		vertices.reserve(xsize*ysize*5);
		indices.reserve(triCount * 5);
		colors.reserve(triCount*3);
	}
  ~ColorGrid();
  // override Shape methods to draw us
  void init() override;
  void render() override;

	void addGrid(float x0, float y0, float dx, float dy);
  void addRectangle(float x, float y, float w, float h);
		
};

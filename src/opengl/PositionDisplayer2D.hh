#pragma once

#include "opengl/Widget2d.hh"

class PositionDisplayer2D : public Widget2d {
private:
	std::vector< Point > points;
public:
	PositionDisplay2D(StyledMultiShape2D* m, MultiText* t, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {}

	void addPoint(float x, float y) {
		points.push_back(Point(x,y))
	}

	void move(int i, float x, float y) {
		points[i].x += x;
		points[i].y += y;
	}

	void init() {
			m->drawPoint(x,y);
	}
}

	class SolarSystemDisplayer : public PositionDisplay2D {
	private:
		std::vector<Body> bodies;
	public:
		void init() {

			m->drawCircle(r, grail::black)
	}

	};

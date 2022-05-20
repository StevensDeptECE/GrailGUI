#include <iostream>
#include <vector>
#include "opengl/Shape.hh"

class StevensCAD : public Shape {
private:
	std::vector<Shape*> shapes;
public:
	StevenCAD();
	~StevensCAD();
	StevensCAD(const StevensCAD& orig) = delete;
	StevensCAD& operator =(const StevensCAD& orig) = delete;

	void init() override;
	void render() override;

	void add(Shape* s);
}

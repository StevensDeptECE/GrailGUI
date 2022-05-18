#include <cmath>
#include <numbers>
using namespace std;
using namespace std::numbers;

void grailmain() {
  Canvas* c = ...;
	StevensCAD* cad = c->addLayer(new StevensCAD());

	cad->add(new Cube(2));

	cad->add(Transform::translate(10,0,0, new Cube(2,3,4)));

	cad->add(Transform::rotate(pi/4, 1,0,0, new Cube(2)));

	cad->add(Transform::rotate(pi/6, 1,1,1, new Cube(2,2,2)));

	cad->add(Transform::rotate(pi/6, 1,1,1,
														 Transform::translate(-10,0,0,new Cube(2,2,2))));
}
	
	

#include "opengl/Shape.hh"
#include "opengl/Canvas.hh"

Shape::~Shape() {}
GLWin* Shape::getWin() const { return parentCanvas->getWin(); }
#include "opengl/MultiShape2D.hh"
#include "opengl/GLWin.hh"
#include "glad/glad.h"
#include "opengl/Shader.hh"
#include "opengl/GLMath.hh"
#include "opengl/Style.hh"
#include "opengl/Canvas.hh"

MultiShape2D::~MultiShape2D() {
}

void MultiShape2D::init(){
  //Create VAO,
  // a container to have all shapes and their attributes
  glGenVertexArrays(1,&vao);
  glBindVertexArray(vao);

  //Create VBO for vertices
  //Create an object in the VAO to store all the vertex values
  glGenBuffers(1,&vbo);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*vertices.size(),&vertices[0],GL_STATIC_DRAW);
  //Desctribe how information is recieved in shaders
  glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,(void*)0);

  //Create SBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as a solid(filled)
  glGenBuffers(1,&sbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*solidIndices.size(),&solidIndices[0],GL_STATIC_DRAW);

  //Create LBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as lines(wireframe)
  glGenBuffers(1,&lbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*lineIndices.size(),&lineIndices[0],GL_STATIC_DRAW);

  //Create PBO
  //Create an object to hold the order at which the vertices are drawn(from indices)
  //in order to draw it as points.
  glGenBuffers(1,&pbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*pointIndices.size(),&pointIndices[0],GL_STATIC_DRAW);
}

//Shape2D
void MultiShape2D::render(){
  //Get Shader based on style
  const Shader* shader = Shader::useShader(style->getShaderIndex());
  //If color buffer exists
  // old projection code
  //  shader->setMat4("projection",*(GLWin::getProjection()));
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());
	
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);


  glLineWidth(style->getLineWidth());

  //Draw Solids
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sbo);
  glDrawElements(GL_TRIANGLES,solidIndices.size(),GL_UNSIGNED_INT,0);

  //Draw Lines
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,lbo);
  glDrawElements(GL_LINES,lineIndices.size(),GL_UNSIGNED_INT,0);

  //Draw Points
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,pbo);
  glDrawElements(GL_POINTS,pointIndices.size(),GL_UNSIGNED_INT,0);

  //Unbind
  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}

//TODO: move common calls to MultiShape
void MultiShape2D::sAddTriIndices(){
  uint32_t startIndex = getPointIndex()-3;
  solidIndices.push_back(startIndex);
  solidIndices.push_back(startIndex+1);
  solidIndices.push_back(startIndex+2);
}

void MultiShape2D::sAddQuadIndices(){
  uint32_t startIndex = getPointIndex()-4;
  solidIndices.push_back(startIndex);
  solidIndices.push_back(startIndex+1);
  solidIndices.push_back(startIndex+2);

  solidIndices.push_back(startIndex+2);
  solidIndices.push_back(startIndex+3);
  solidIndices.push_back(startIndex);
}

void MultiShape2D::sAddSectorIndices(uint32_t centerIndex, uint32_t indexCount){
  uint32_t startIndex = getPointIndex()-indexCount;
  for(uint32_t curIndex = startIndex;curIndex<startIndex+indexCount-1;curIndex++){
    solidIndices.push_back(centerIndex);
    solidIndices.push_back(curIndex);
    solidIndices.push_back(curIndex+1);
  }
}

void MultiShape2D::lAddTriIndices(){
  uint32_t startIndex = getPointIndex()-3;
  lineIndices.push_back(startIndex); lineIndices.push_back(startIndex+1);
  lineIndices.push_back(startIndex+1); lineIndices.push_back(startIndex+2);
  lineIndices.push_back(startIndex+2); lineIndices.push_back(startIndex);
}

void MultiShape2D::lAddQuadIndices(){
  uint32_t startIndex = getPointIndex()-4;
  lineIndices.push_back(startIndex); lineIndices.push_back(startIndex+1);
  lineIndices.push_back(startIndex+1); lineIndices.push_back(startIndex+2);
  lineIndices.push_back(startIndex+2); lineIndices.push_back(startIndex+3);
  lineIndices.push_back(startIndex+3); lineIndices.push_back(startIndex);
}

void MultiShape2D::lAddSectorIndices(uint32_t centerIndex, uint32_t indexCount){
  uint32_t startIndex = getPointIndex()-indexCount;
  for(uint32_t curIndex = startIndex+1;curIndex<startIndex+indexCount-1;curIndex++){
    lineIndices.push_back(curIndex); lineIndices.push_back(curIndex+1);
  }
}

void MultiShape2D::pAddTriIndices(){
  uint32_t startIndex = getPointIndex()-3;

  pointIndices.push_back(startIndex);
  pointIndices.push_back(startIndex+1);
  pointIndices.push_back(startIndex+2);
}

void MultiShape2D::pAddQuadIndices(){
  uint32_t startIndex = getPointIndex()-4;

  pointIndices.push_back(startIndex);
  pointIndices.push_back(startIndex+1);
  pointIndices.push_back(startIndex+2);
  pointIndices.push_back(startIndex+3);
}

void MultiShape2D::pAddSectorIndices(uint32_t centerIndex, uint32_t indexCount){
  uint32_t startIndex = getPointIndex()-indexCount;
  for(uint32_t curIndex = startIndex;curIndex<startIndex+indexCount-1;curIndex++){
    pointIndices.push_back(curIndex);
  }
}

/*
	Draw a sector of an ellipse aligned to the xy axes
	centered on (xc, yc) with radius xRad in the x-direction, yRad in the y
	fromAngle to toAngle stepping by angleInc
  
  RETURNS: the number of points that make up the sector and
  have been added as (x,y) pairs to the vertices vector

	all angles specified in degrees
 */
uint32_t MultiShape2D::addSector(float xc, float yc, float xRad, float yRad,
																 float fromAngle, float toAngle, float angleInc){
  uint32_t points_added = 0;
  for(float i = fromAngle; i<=toAngle; i += angleInc) {
    float px = float (xc + xRad*cos(-i*DEG2RAD<double>));
    float py = float (yc + yRad*sin(-i*DEG2RAD<double>));
		
    addPoint(px,py);
    points_added++;
	}
  return points_added;
}

//Solid Primitives
void MultiShape2D::fillRectangle(float x, float y, float w, float h){
  addPoint(x,y);  	  addPoint(x,y+h); // goes in counter-clockwise order
  addPoint(x+w, y+h); addPoint(x+w, y);
  sAddQuadIndices();
}

void MultiShape2D::fillRoundRect(float x, float y, float w, float h,
																 float rx, float ry) {
	//TODO: add color in each method from the style
  uint32_t centerIndex = getPointIndex();
  addPoint(x+w/2,y+h/2);

  uint32_t toAdd = addSector(x+rx,y+ry,rx,ry,90,180,1);
  sAddSectorIndices(centerIndex,toAdd);

  solidIndices.push_back(centerIndex);
  solidIndices.push_back(getPointIndex()-1);
  solidIndices.push_back(getPointIndex());

  toAdd = addSector(x+rx,y+h-ry,rx,ry,180,270,1);
  sAddSectorIndices(centerIndex,toAdd);

  solidIndices.push_back(centerIndex);
  solidIndices.push_back(getPointIndex()-1);
  solidIndices.push_back(getPointIndex());

  toAdd = addSector(x+w-rx,y+h-ry,rx,ry,270,360,1);
  sAddSectorIndices(centerIndex,toAdd);

  solidIndices.push_back(centerIndex);
  solidIndices.push_back(getPointIndex()-1);
  solidIndices.push_back(getPointIndex());

  toAdd = addSector(x+w-rx,y+ry,rx,ry,0,90,1);
  sAddSectorIndices(centerIndex,toAdd);

  solidIndices.push_back(centerIndex);
  solidIndices.push_back(getPointIndex()-1);
  solidIndices.push_back(centerIndex+1);
}

void MultiShape2D::fillTriangle(float x1, float y1, float x2, float y2,float x3, float y3){
  addPoint(x1,y1);   addPoint(x2, y2);  addPoint(x3,y3);
  sAddTriIndices();
}

void MultiShape2D::fillPolygon(float x, float y, float xRad, float yRad, float n){
  uint32_t centerIndex = getPointIndex();
  addPoint(x,y);

  uint32_t toAdd = addSector(x,y,xRad,yRad,0,360,360/n);
  sAddSectorIndices(centerIndex,toAdd);
}

void MultiShape2D::fillCircle(float x ,float y, float rad, float angleInc){
  uint32_t centerIndex = getPointIndex();
  addPoint(x,y);

  uint32_t toAdd = addSector(x,y,rad,rad,0,360,angleInc);
  sAddSectorIndices(centerIndex,toAdd);
}

void MultiShape2D::fillEllipse(float x, float y, float xRad, float yRad, float angleInc){
  uint32_t centerIndex = getPointIndex();
  addPoint(x,y);

  uint32_t toAdd = addSector(x,y,xRad,yRad,0,360,angleInc);
  sAddSectorIndices(centerIndex,toAdd);
}

//Line Primitives
void MultiShape2D::drawRectangle(float x, float y, float w, float h){
  addPoint(x,y);  	  addPoint(x,y+h); // goes in counter-clockwise order
  addPoint(x+w, y+h); addPoint(x+w, y);
  lAddQuadIndices();
}

void MultiShape2D::drawRoundRect(float x, float y, float w, float h, float rx,float ry){
  uint32_t centerIndex = getPointIndex();
  addPoint(x+w/2,y+h/2);

  uint32_t toAdd = addSector(x+rx,y+ry,rx,ry,90,180,1);
  lAddSectorIndices(centerIndex,toAdd);

  lineIndices.push_back(getPointIndex()-1);
  lineIndices.push_back(getPointIndex());

  toAdd = addSector(x+rx,y+h-ry,rx,ry,180,270,1);
  lAddSectorIndices(centerIndex,toAdd);

  lineIndices.push_back(getPointIndex()-1);
  lineIndices.push_back(getPointIndex());

  toAdd = addSector(x+w-rx,y+h-ry,rx,ry,270,360,1);
  lAddSectorIndices(centerIndex,toAdd);

  lineIndices.push_back(getPointIndex()-1);
  lineIndices.push_back(getPointIndex());

  toAdd = addSector(x+w-rx,y+ry,rx,ry,0,90,1);
  lAddSectorIndices(centerIndex,toAdd);

  lineIndices.push_back(getPointIndex()-1);
  lineIndices.push_back(centerIndex+1);
}

void MultiShape2D::drawTriangle(float x1, float y1, float x2, float y2,float x3, float y3){
  addPoint(x1,y1);   addPoint(x2, y2);  addPoint(x3,y3);
  lAddTriIndices();
}

void MultiShape2D::drawPolygon(float x, float y, float xRad, float yRad, float n){
  uint32_t centerIndex = getPointIndex();
  addPoint(x,y);

  uint32_t toAdd = addSector(x,y,xRad,yRad,0,360,360/n);
  lAddSectorIndices(centerIndex,toAdd);
}

void MultiShape2D::drawCompletePolygon(float x, float y, float xRad, float yRad, float n){

}

void MultiShape2D::drawCircle(float x ,float y, float rad, float angleInc){
  drawEllipse(x,y,rad,rad,angleInc);
}

void MultiShape2D::drawEllipse(float x, float y, float xRad, float yRad, float angleInc){
  uint32_t centerIndex = getPointIndex();
  addPoint(x,y);

  uint32_t toAdd = addSector(x,y,xRad,yRad,0,360,angleInc);
  lAddSectorIndices(centerIndex,toAdd);
  lineIndices.push_back(getPointIndex()-1);
  
  lineIndices.push_back(centerIndex+2);
}

void MultiShape2D::drawLine(float x1, float y1, float x2,float y2){
  addLine(x1,y1,x2,y2);
}
//drawBezier, end bezier,grid

//Point Primitives
void MultiShape2D::rectanglePoints(float x, float y, float w, float h){
  addPoint(x,y);  	  addPoint(x,y+h); // goes in counter-clockwise order
  addPoint(x+w, y+h); addPoint(x+w, y);
  pAddQuadIndices();
}

void MultiShape2D::roundRectPoints(float x, float y, float w, float h, float rx,float ry){
  uint32_t centerIndex = getPointIndex();
  addPoint(x+w/2,y+h/2);

  uint32_t toAdd = addSector(x+rx,y+ry,rx,ry,90,180,1);
  pAddSectorIndices(centerIndex,toAdd);

  toAdd = addSector(x+rx,y+h-ry,rx,ry,180,270,1);
  pAddSectorIndices(centerIndex,toAdd);

  toAdd = addSector(x+w-rx,y+h-ry,rx,ry,270,360,1);
  pAddSectorIndices(centerIndex,toAdd);

  toAdd = addSector(x+w-rx,y+ry,rx,ry,0,90,1);
  pAddSectorIndices(centerIndex,toAdd);
}

void MultiShape2D::trianglePoints(float x1, float y1, float x2, float y2,float x3, float y3){
  addPoint(x1,y1);   addPoint(x2, y2);  addPoint(x3,y3);
  pAddTriIndices();
}

void MultiShape2D::polygonPoints(float x, float y, float xRad, float yRad, float n){
  uint32_t centerIndex = getPointIndex();
  addPoint(x,y);

  uint32_t toAdd = addSector(x,y,xRad,yRad,0,360,360/n);
  pAddSectorIndices(centerIndex,toAdd);
}

void MultiShape2D::circlePoints(float x ,float y, float rad, float angleInc){
  uint32_t centerIndex = getPointIndex();
  addPoint(x,y);

  uint32_t toAdd = addSector(x,y,rad,rad,0,360,angleInc);
  pAddSectorIndices(centerIndex,toAdd);
}

void MultiShape2D::ellipsePoints(float x, float y, float xRad, float yRad, float angleInc){
  uint32_t centerIndex = getPointIndex();
  addPoint(x,y);

  uint32_t toAdd = addSector(x,y,xRad,yRad,0,360,angleInc);
  pAddSectorIndices(centerIndex,toAdd);
}

void MultiShape2D::drawPoint(float x, float y){
    uint32_t ind = getPointIndex();
    addPoint(x, y);
    pointIndices.push_back(ind);
}

/*
   Draw a single segment of a bezier curve
 */
void MultiShape2D::bezierSegment(const Bezier* b) {

	drawPoint(b->getp2x(), b->getp2y());
	drawPoint(b->getp3x(), b->getp3y());
	uint32_t ind = getPointIndex();
	pointIndices.push_back(ind);
	double t = 0;
	for (uint32_t i = 0; i < b->getn(); i++, t += b->getdt()) {
		float x = ((b->getax() * t + b->getbx()) * t + b->getcx()) * t + b->getdx();
		float y = ((b->getay() * t + b->getby()) * t + b->getcy()) * t + b->getdy();
		addPoint(x, y);
		lineIndices.push_back(ind);
		lineIndices.push_back(++ind);
	}
	if (b->getEnd() == true){
		pointIndices.push_back(ind);
		addPoint(b->getp4x(), b->getp4y());
	}else{
		lineIndices.pop_back();
		ind += 2;
		lineIndices.push_back(ind);
		pointIndices.push_back(ind);
	}
}

void MultiShape2D::bezierSegmentByPoints(float p1x, float p1y, float p2x, float p2y,
																				 float p3x, float p3y, float p4x, float p4y,
																				 int n, bool end){
	float ax = -p1x + 3 * p2x - 3 * p3x + p4x;  float ay = -p1y + 3 * p2y - 3 * p3y + p4y;
	float bx = 3 * p1x - 6 * p2x + 3 * p3x;     float by = 3 * p1y - 6 * p2y + 3 * p3y;
	float cx = -3 * p1x + 3 * p2x;              float cy = -3 * p1y + 3 * p2y;
	Bezier b(ax, bx, cx, p1x, ay, by, cy, p1y, n, end);
	bezierSegment(&b);
}

void MultiShape2D::spline(const std::vector<double> &points, int n){
	uint32_t num = points.size() / 2 - 1;
	std::vector<double> parameter, dx, dy;
	
	parameter.push_back(4.0);
	for(uint32_t i = 1; i < num - 1; i++){
		parameter.push_back(4.0 - 1.0 / parameter[i - 1]);
	}
	parameter.push_back(4.0 - 1.0 / parameter[num - 2]);
	
	dx.push_back(3 * (points[2 * num] - points[2 * num - 2]) / parameter[num - 1]);
	dy.push_back(3 * (points[2 * num + 1] - points[2 * num - 1]) / parameter[num - 1]);
	for(uint32_t i = num - 2; i > 0; i--){
		double d = (3 * (points[2 * i + 2] - points[2 * i - 2]) - dx[0]) / parameter[i - 1];
		dx.insert(dx.begin(), d);
		d = (3 * (points[2 * i + 3] - points[2 * i - 1]) - dy[0]) / parameter[i - 1];
		dy.insert(dy.begin(), d);
	}
	dx.insert(dx.begin(), (3 * (points[2] - points[0]) - dx[0]) / parameter[0]);
	dy.insert(dy.begin(), (3 * (points[3] - points[1]) - dy[0]) / parameter[0]);
	
	for(uint32_t i = 0; i < num; i++){
		float ax = float(2 * (points[2 * i] - points[2 * i + 2]) + dx[i] + dx[i + 1]);
		float bx = float(3 * (points[2 * i + 2] - points[2 * i]) - 2 * dx[i] - dx[i + 1]);
		float cx = float(dx[i]);
		float dx = float(points[2 * i]);
		float ay = float(2 * (points[2 * i + 1] - points[2 * i + 3]) + dy[i] + dy[i + 1]);
		float by = float(3 * (points[2 * i + 3] - points[2 * i + 1]) - 2 * dy[i] - dy[i + 1]);
		float cy = float(dy[i]);
		float dy = float(points[2 * i + 1]);
		if(i == num - 1){
			Bezier b(ax, bx, cx, dx,  ay, by, cy, dy, n, true);
			bezierSegment(&b);
		}else{
			Bezier b(ax, bx, cx, dx,  ay, by, cy, dy, n, false);
			bezierSegment(&b);
		}
	}
}




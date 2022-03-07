#include "CAD/BezierPath.hh"
#include "CAD/Vec3d.hh"
//#include "CAD/Matrix3d.hh"

using namespace std;

// double slope(Vec3D p1, Vec3D p2){ //copying large objects (slow) const Vec3D& p1, const Vec3D& p2
//   return (p2.y - p1.y)/(p2.x - p1.x);
// }
// void Bezier::getEquationX(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
//   double dx10 = p1.x - p0.x;
//   double dx32 = p3.x - p2.x;
//   dx = p0.x;
//   cx = dx10;
//   bx = -3*(p3.x) + 3*(p0.x) + 2*(dx10) + dx32;
//   ax = p3.x - p0.x - dx10 + 3*(p3.x) - 3*(p0.x) - 2*(dx10) + dx32;
// }

// //pX -> point[0] so no input?
// void Bezier::getEquationY(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
//   dy = p0.y;
//   cy = slope(p0, p1);
//   by = -3*(p3.y) + 3*(p0.y) + 2*(slope(p0, p1)) + slope(p2, p3);
//   ay = p3.y - p0.y - slope(p0, p1) + 3*(p3.y) - 3*(p0.y) - 2*(slope(p0, p1)) + slope(p2, p3);
// }

// void Bezier::getEquationZ(const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
//   dz = p0.z;
//   cz = slope(p0, p1);
//   bz = -3*(p3.z) + 3*(p0.z) + 2*(slope(p0, p1)) + slope(p2, p3);
//   az = p3.z - p0.z - slope(p0, p1) + 3*(p3.z) - 3*(p0.z) - 2*(slope(p0, p1)) + slope(p2, p3);
// }

void BezierPath::getEquationX(double t, const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
  double t2 = t*t;
  double t3= t2*t;
  double dx10 = p1.x - p0.x;
  double dx12 = p1.x - p2.x;
  dx = p0.x;
  cx = 3*t*(dx10);
  bx = 3*t2*(p2.x + p0.x - 2*p1.x);
  ax = t3*(p3.x+3*(dx12)-p0.x);
}

void BezierPath::getEquationY(double t, const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
  double t2 = t*t;
  double t3= t2*t;
  double dy10 = p1.y - p0.y;
  double dy12 = p1.y - p2.y;
  dy = p0.y;
  cy = 3*t*(dy10);
  by = 3*t2*(p2.y + p0.y - 2*p1.y);
  ay = t3*(p3.y+3*(dy12)-p0.y); 
}

void BezierPath::getEquationZ(double t, const Vec3D& p0, const Vec3D& p1, const Vec3D& p2, const Vec3D& p3){
  double t2 = t*t;
  double t3= t2*t;
  double dz10 = p1.z - p0.z;
  double dz12 = p1.z - p2.z;
  dz = p0.z;
  cz = 3*t*(dz10);
  bz = 3*t2*(p2.z + p0.z - 2*p1.z);
  az = t3*(p3.z+3*(dz12)-p0.z); 
}

float BezierPath::x(double t)  {
  //cout << "ax: " << ax << ", bx: " << bx << ", cx: "<< cx << ", dx: " << dx << endl;
  this->getEquationX(t, p1, p2, p3, p4);
  return ((t*ax + bx) * t + cx) * t + dx;
}

float BezierPath::y(double t) {
  this->getEquationY(t, p1, p2, p3, p4);
  return ((t*ay + by) * t + cy) * t + dy;
}

float BezierPath::z(double t) {
  this->getEquationZ(t, p1, p2, p3, p4);
  return ((t*az + bz) * t + cz) * t + dz;
}

// Vec3D Bezier::getPoint(double step){
//   return Vec3D(x(step), y(step), z(step));
// }

//computePoints
void BezierPath::getPoints(){
  for(double i=0; i<=1; i+=0.1){
      cout<< Vec3D(x(i), y(i), z(i)) << endl;
      points.push_back(Vec3D(x(i), y(i), z(i)));
  }
}

std::vector<float> BezierPath::unwrap(std::vector<Vec3D> x){
  std::vector<float> temp(x.size()*3);
  for (int i=0, j=0; i<x.size(); i++){
    temp[j++] = x[i].x;
    temp[j++] = x[i].y;
    temp[j++] = x[i].z; //0, when z != 0 doesn't work
  }
  return temp;
}

void BezierPath::init(){
  //create VAO to hold shapes
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  //create VBO to hold vertex points
  drawingPoints = unwrap(points);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, drawingPoints.size() * sizeof(float), &drawingPoints[0], GL_STATIC_DRAW);

  //describe in shaders
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void BezierPath::render(){
  // for(int i=0; i<drawingPoints.size(); i++){
  //   cout << drawingPoints[i] << " " ;
  // }
  // cout << endl;

  Shader * shader = Shader::useShader(GLWin::COMMON_SHADER);
  shader->setMat4("projection",*(parentCanvas->getProjection()));
	shader->setVec4("solidColor",style->getFgColor());
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  glLineWidth(style->getLineWidth());
  //cout<< drawingPoints.size()/3 <<endl;
  glDrawArrays(GL_LINE_STRIP, 0, drawingPoints.size()/3);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);

}

// int main(){
//   Vec3D a(0,0,0);
//   Vec3D b(2,5,0);
//   Vec3D c(5,0,0);
//   Vec3D d(7,5,0);

//   Bezier b1(a,b,c,d);
  
//   //check that unwrap works
//   // for (int i=0; i<b1.drawingPoints.size(); i++){
//   //   cout<< b1.drawingPoints[i] << endl;
//   // }
// }

//references:
//https://www.google.com/search?q=how+to+Compute+the+coordinates+for+the+formula+of+a+circle+3D&rlz=1C1CHBF_enUS872US872&oq=how+to+Compute+the+coordinates+for+the+formula+of+a+circle+3D&aqs=chrome..69i57.2310j0j4&sourceid=chrome&ie=UTF-8
//https://javascript.info/bezier-curve

//#include "CAD/Helix.cc"
#include "CAD/Triangle.hh"
#include "CAD/Circle.hh"
#include "CAD/Line.hh"
#include "CAD/Facet.hh"

#include "CAD/Vec3d.hh"

#include "CAD/BezierPath.hh"
#include "opengl/StyledMultiShape2D.hh"

#include <iostream>
#include "opengl/GrailGUI.hh"
#include "opengl/util/Camera.hh"

#include <vector>

using namespace std;

class CurveTest : public GLWin {
  private:
    Style *baseGraphStyle;

  public:
    CurveTest() : GLWin(0x000000, 0xCCCCCC, "CurveTest") {}

  void init(){
    cout<< "IN NEW CURVETEST" << endl;
    baseGraphStyle = new Style("TIMES", 12, 1, 0, 0, 0, 0, 0, 0);
    baseGraphStyle->setLineWidth(10);
  
    //set the camera
    MainCanvas *c = currentTab()->getMainCanvas();
    Camera* cam = c->setLookAtProjection(20, 0, 50, 0, 0, 0, 0, 1, 0);
    c->setProjection(cam->getViewProjection());

    //rectangle test
    //StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, baseGraphStyle));
    //m->fillRectangle(0,0,100,100,grail::red);
    
    //helix test
    // Vec3D start(0,0,0);
    // Helix* test = new Helix(start, 10, 2, c, baseGraphStyle);
    // c->addLayer(test);

    

    // c->addLayer(btest);
    //cout << "new curve test" << endl;
    

    // Triangle* tritest = new Triangle(c,2,2,2,3,baseGraphStyle);
    // c->addLayer(tritest);

    std::vector<Vec3D> points;
    Vec3D f1(3,0,0);
    Vec3D f2(3,3,0);
    Vec3D f3(6,0,0);
    Vec3D f4(6,3,0);

    points.push_back(f1);
    points.push_back(f2);
    points.push_back(f3);
    //points.push_back(f4);

    Facet* ftest = new Facet(points,c,baseGraphStyle);
    c->addLayer(ftest);

    //bezier test
    Vec3D b1(3,0,0);
    Vec3D b2(6,7,0);
    Vec3D b3(9,0,0);
    Vec3D b4(12,7,0);
    BezierPath* btest = new BezierPath(b1,b2,b3,b4,c,baseGraphStyle);
    // cout<< "BPOITNS: " << btest->points[0] <<", " << btest->points[size(btest->points)-1] << endl;
    cout<< "BPOITNS: " << size(btest->points) << endl;
    c->addLayer(btest);

    //cout<< (btest->points[size(btest->points)-1]).x << endl;
    double t = size(btest->points)-1;
    double xn = (btest->points[size(btest->points)-1]).x/(size(btest->points)-1);
    double yn= (btest->points[size(btest->points)-1]).y/(size(btest->points)-1);
    double zn = (btest->points[size(btest->points)-1]).z/(size(btest->points)-1);
    cout << xn <<", " << yn << ", " << zn << endl;
    for (double i=0; i<=t; i+=1){
      std::vector<Vec3D> points2;
      Vec3D m(xn*i,yn*i,zn*i);
      cout << i << ": " << m << endl;
      Vec3D f5=f1+m;
      Vec3D f6=f2+m;
      Vec3D f7=f3+m;
      points2.push_back(f5);
      points2.push_back(f6);
      points2.push_back(f7);
      Facet* ftest2 = new Facet(points2,c,baseGraphStyle);
      c->addLayer(ftest2);
    }
   
    // Line* ltest = new Line(f1,f2,c, baseGraphStyle);
    // c->addLayer(ltest);
    // Line* ltest2 = new Line(f2,f3,c, baseGraphStyle);
    // c->addLayer(ltest2);
    // Line* ltest3 = new Line(f3,f1,c, baseGraphStyle);
    // c->addLayer(ltest3);

  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new CurveTest(), 1000, 1000);
}

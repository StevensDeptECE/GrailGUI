//#include "grail/Grail.hh"
#include "opengl/Colors.hh"
#include "opengl/util/Transformation.hh"
using namespace std;

class TestDrawing3D : public GLWin {
 private:
  Transformation tBoxes;
  Transformation tFrog;
  float vx, vy, g;
  float t;
 public:
  void init() {
    g = -9.8; // gravity, acting down
    vx = 5.4;
    vy = 8;
    t = 0;
    Style* s = getDefaultStyle();
    Canvas* c = currentTab()->getMainCanvas();

    Camera* cam = new Camera(width, height);
    cam->setCamPos(glm::vec3(0,4, 22));

    vector<const char*> blockTextures;
    blockTextures.push_back("textures/water.jpg");
    blockTextures.push_back("textures/wood.jpg");

//TODO: Make a single function that creates a textured cube, and same for other shapes.
    vector<pair<float, float>> blockTexCoords;
    for (int i = 0; i < 8; i++) {
      blockTexCoords.push_back(make_pair(0.54537f, 1.0f - 0.000004f));
      blockTexCoords.push_back(make_pair(0.000103f, 1.0f - 0.336048f));
      blockTexCoords.push_back(make_pair(0.335973f, 1.0f - 0.335903f));
    }

    tBoxes.translate(0,0,10);
    
    MultiShape3D* m = c->addLayer(new MultiShape3D(s, cam, blockTextures, &tBoxes));
    m->genRectPrism(-5.0f, 0, 0, 4, 4, 1, 1, blockTexCoords);
    m->genRectPrism( 0.0f, 0, 0, 4, 4, 1, 0, blockTexCoords);
    m->genRectPrism( 5.0f, 0, 0, 4, 4, 1, 1, blockTexCoords);

    vector<const char*> frogTextures;
    frogTextures.push_back("textures/frog-eye.jpg");
    frogTextures.push_back("textures/frog-tex.jpg");

    tFrog.translate(-1,1.8,3);
    MultiShape3D* m2 = c->addLayer(new MultiShape3D(s, cam, frogTextures, &tFrog));
    vector<uint32_t> texIndices;
    texIndices.push_back(0);
    texIndices.push_back(1);
    m2->genOBJModel("models/frog.obj", texIndices);
  }
  void update() {
    if (t < 0.85) {
      float x = vx*t;
      float y = (vy + g*t)*t;
      tFrog.ident();
      tFrog.translate(-1,1.8,3);
      tFrog.translate(x, y, 0);
      t += .005;
    }
    
  }
};

int main(int argc, char* argv[]) {
  return GLWin::init(new TestDrawing3D());
}

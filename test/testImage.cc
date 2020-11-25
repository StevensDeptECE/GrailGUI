#include "grail/Grail.hh"
#include "opengl/primitivesV2/Colors.hh"
using namespace std;

class TestImage : public Grail {
 public:
  TestImage(const char filename[]){
    images.push_back("right.png");
    images.push_back("right.png");
    images.push_back("right.png");
  }
  std::vector<std::string> images;

  void update()
	{
		currentTab()->update(); 
	}
  void init() {
    Style* s = getDefaultStyle();
    Canvas* c = currentTab()->getMainCanvas();
    Image::combineImage(images);
    Image* image = new Image(200, 300, 300, 500, "test.jpg", s);
    c->addLayer(image);
    //image->addImage(900,500, 100,100, 0,0.5f, 0.25f,0.67f);  //! Logo
    //image->addImage(800,400, 100,100, .375f,.5f, .625f,.7f); //! Face
    
  }
};

int main(int argc, char* argv[]) {
  return Grail::init(new TestImage(argv[1]), "out.cml");
}

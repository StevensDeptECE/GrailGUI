#include <cstdlib>
#include <glm/glm.hpp>

#include "opengl/GrailGUI.hh"
using namespace std;

class JoeyDrawing : public GLWin {
 public:
  void init() {
    const Style* s = getDefaultStyle();
    Canvas* c = currentTab()->addCanvas(s, 0, 300, width, height - 300);
    StyledMultiShape2D* m = c->addLayer(new StyledMultiShape2D(c, s));

    // Variables for color cycling
    bool flagr = false;
    bool flagg = false;
    bool flagb = false;
    bool flagn = false;

    float r = 0;
    float g = 0;
    float b = 0;
    float n = 0;

    for (float i = 0.0f; i <= 4000.0f; i += .5f) {
      m->drawCircle(width * .5, height * .5, i, 5,
                    glm::vec4((r / 255.0f), (g / 255.0f), (b / 255.0f), 0));
      if ((int(i) % 3) == 0) {
        if (r < 255.0f) {
          if (flagr == true) {
            r = (r - 1.0f);
          } else if (flagr == false) {
            r = (r + 1.0f);
          }
        } else if (r == 255.0f) {
          flagr = true;
          r = (r - 1.0f);
        } else if (r == 0.0f) {
          flagr = false;
          r = (r + 1.0f);
        }
      }
      if ((int(i) % 5) == 0) {
        if (g < 255.0f) {
          if (flagg == true) {
            g = (g - 1.0f);
          } else if (flagg == false) {
            g = (g + 1.0f);
          }
        } else if (g == 255.0f) {
          flagg = true;
          g = (g - 1.0f);
        } else if (g == 0.0f) {
          flagg = false;
          g = (g + 1.0f);
        }
      }
      if ((int(i) % 2) == 0) {
        if (b < 255.0f) {
          if (flagb == true) {
            b = (b - 1.0f);
          } else if (flagb == false) {
            b = (b + 1.0f);
          }
        } else if (b == 255.0f) {
          flagb = true;
          b = (b - 1.0f);
        } else if (b == 0.0f) {
          flagb = false;
          b = (b + 1.0f);
        }
      }
      /* n = upanddown(n, flagn); */
    }
  }
};

int main(int argc, char* argv[]) { return GLWin::init(new JoeyDrawing()); }

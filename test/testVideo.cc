#include "audio/VideoPlayer.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestVideoPlayer : public GLWin {
 private:
  double startTime;
  VideoPlayer *v;
  int step;

 public:
  TestVideoPlayer()
      : GLWin(0x000000, 0xCCCCCC, "TestVideoPlayer"),
        startTime(0),
        v(nullptr),
        step(0) {}

  ~TestVideoPlayer() { delete v; }

  void update() {
    int w, h;
    auto param = (mpv_opengl_fbo){
        .fbo = 0,
        .w = 100,
        .h = 100,
    };
    mpv_render_param params[] = {{MPV_RENDER_PARAM_OPENGL_FBO, &param},
                                 {MPV_RENDER_PARAM_FLIP_Y, (int *)1},
                                 (mpv_render_param_type)0};

    mpv_render_context_render(v->mpv_gl, params);
    double time = getTime();

    if (time > startTime + 1 && step == 0) {
    }
  }

  void init() {
    startTime = getTime();
    MainCanvas *c = currentTab()->getMainCanvas();

    StyledMultiShape2D *m = c->getGui();

    v = new VideoPlayer();

    v->setVid("res/sample1mb.ogg");
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestVideoPlayer(), 300, 300);
}

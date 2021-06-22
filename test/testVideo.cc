#include "audio/VideoPlayer.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestVideoPlayer : public GLWin {
 private:
  double startTime;
  VideoPlayer *v;
  int step;
  mpv_opengl_fbo *fboParams;
  int paramFlip;
  uint64_t flags;

 public:
  TestVideoPlayer()
      : GLWin(0x000000, 0xCCCCCC, "TestVideoPlayer"),
        startTime(0),
        v(nullptr),
        step(0) {}

  ~TestVideoPlayer() { delete v; }

  void renderVideo() {
    mpv_render_param params[] = {
        // Specify the default framebuffer (0) as target. This will
        // render onto the entire screen. If you want to show the video
        // in a smaller rectangle or apply fancy transformations, you'll
        // need to render into a separate FBO and draw it manually.
        {MPV_RENDER_PARAM_OPENGL_FBO, fboParams},
        // Flip rendering (needed due to flipped GL coordinate system).
        {MPV_RENDER_PARAM_FLIP_Y, &paramFlip},
        (mpv_render_param_type)0};
    // See render_gl.h on what OpenGL environment mpv expects, and
    // other API details.
    mpv_render_context_render(v->mpv_gl, params);
  }

  void update() {
    flags = mpv_render_context_update(v->mpv_gl);
    if (flags & MPV_RENDER_UPDATE_FRAME) setDirty();
    // double time = getTime();

    // if (time > startTime + 1 && step == 0) {
    // }
  }

  void init() {
    startTime = getTime();
    MainCanvas *c = currentTab()->getMainCanvas();

    StyledMultiShape2D *m = c->getGui();

    paramFlip = 1;

    fboParams = new mpv_opengl_fbo;
    fboParams->fbo = 0;
    fboParams->w = 300;
    fboParams->h = 300;

    v = new VideoPlayer();

    // playing a link from youtube (and maybe other places idk) requires you to
    // have youtube-dl installed
    v->setVid("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    v->togglePause();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestVideoPlayer(), 300, 300);
}

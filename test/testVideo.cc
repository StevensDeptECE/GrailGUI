#include "opengl/GrailGUI.hh"
#include "video/VideoPlayer.hh"

using namespace std;
using namespace grail;

class TestVideoPlayer : public GLWin {
 private:
  double startTime;
  VideoPlayer *v;

 public:
  TestVideoPlayer()
      : GLWin(0x000000, 0xCCCCCC, "TestVideoPlayer"),
        startTime(0),
        v(nullptr) {}

  ~TestVideoPlayer() { delete v; }

  void update() {
    auto flags = mpv_render_context_update(v->mpv_gl);
    if (flags & MPV_RENDER_UPDATE_FRAME) setDirty();
    // double time = getTime();

    // if (time > startTime + 1 && step == 0) {
    // }
  }

  void init() {
    startTime = getTime();
    MainCanvas *c = currentTab()->getMainCanvas();

    v = c->addLayer(new VideoPlayer(c, 100, 100, 500, 500));

    // playing a link from youtube (and maybe other places idk) requires you to
    // have youtube-dl installed
    v->loadFile(
        "https://www.youtube.com/"
        "playlist?list=PLFsQleAWXsj_4yDeebiIADdH5FMayBiJo");
    v->togglePause();
    v->cropVideo(0, 0, 60, 60);
    v->setVolume(50);
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestVideoPlayer(), 1000, 1000);
}

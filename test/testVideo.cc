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

  void update() { v->update(); }

  void init() {
    startTime = getTime();
    MainCanvas *c = currentTab()->getMainCanvas();

    v = c->addLayer(new VideoPlayer(c, 100, 100, 640, 360));

    // playing a link from youtube (and maybe other places idk) requires you to
    // have youtube-dl installed
    v->loadFile("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    v->cropVideo(0, 0, 0, 0);
    v->setVolume(50);
    v->setPlaying();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestVideoPlayer(), 1000, 1000);
}

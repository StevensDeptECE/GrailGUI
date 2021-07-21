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

  void update() { v->update(); }

  void init() {
    startTime = getTime();
    MainCanvas *c = currentTab()->getMainCanvas();

    v = c->addLayer(new VideoPlayer(c, 100, 100, 500, 500));

    // playing a link from youtube (and maybe other places idk) requires you to
    // have youtube-dl installed
    v->loadFile("res/badapple.mkv");
    v->cropVideo(0, 0, 65, 65);
    v->setVolume(50);
    v->setPlaying();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestVideoPlayer(), 1000, 1000);
}

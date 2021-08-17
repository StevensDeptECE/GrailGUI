#include "opengl/Animated.hh"
#include "opengl/GrailGUI.hh"
#include "video/VideoPlayer.hh"

using namespace std;
using namespace grail;

class TestVideoPlayer : public Animated {
 private:
  VideoPlayer* v;

 public:
  TestVideoPlayer(Tab* tab) : Animated(tab), v(nullptr) {}

  void update() { v->update(); }

  void init() {
    v = c->addLayer(new VideoPlayer(c, 100, 100, 500, 500));

    // playing a link from youtube (and maybe other places idk) requires you to
    // have youtube-dl installed
    v->loadFile("/home/alice/grail.mkv");
    v->cropVideo(0, 0, 65, 65);
    v->setVolume(50);
    v->setPlaying();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  defaultTab->addAnimated(new TestVideoPlayer(defaultTab));
}

#include "opengl/GrailGUI.hh"
#include "video/VideoPlayer.hh"

using namespace std;
using namespace grail;

class TestVideoPlayer : public Member {
 public:
  TestVideoPlayer(Tab* tab) : Member(tab) {
    VideoPlayer* v = c->addLayer(new VideoPlayer(c, 100, 100, 500, 500));

    // playing a link from youtube (and maybe other places idk) requires you to
    // have youtube-dl installed
    v->loadFile("https://www.youtube.com/watch?v=ZlPpHILyEl4");
    // v->cropVideo(0, 0, 65, 65);
    v->setVolume(50);
    v->setPlaying();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  defaultTab->addMember(new TestVideoPlayer(defaultTab));
}

#include "opengl/GrailGUI.hh"
#include "video/VideoPlayer.hh"

using namespace std;
using namespace grail;

class TestVideoPlayer : public Member {
 public:
  TestVideoPlayer(Tab* tab) : Member(tab, 0) {
    VideoPlayer* v = c->addLayer(new VideoPlayer(c, 100, 100, 500, 500));

    // playing a link from youtube (and maybe other places idk) requires you to
    // have youtube-dl installed
    v->loadFile("https://sample-videos.com/video123/mp4/720/big_buck_bunny_720p_1mb.mp4");
    //v->loadFile("Downloads/SampleVideo_1280x720_1mb.mp4");
    // v->cropVideo(0, 0, 65, 65);
    v->setVolume(50);
    v->setPlaying();
  }
};

void grailmain(int argc, char* argv[], GLWin* w, Tab* defaultTab) {
  defaultTab->addMember(new TestVideoPlayer(defaultTab));
}

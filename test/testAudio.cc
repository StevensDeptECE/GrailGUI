#include "audio/AudioPlayer.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestAudioPlayer : public GLWin {
 private:
  double startTime;
  AudioPlayer *a;
  bool playing;
  bool didit;

 public:
  TestAudioPlayer()
      : GLWin(0x000000, 0xCCCCCC, "TestAudioPlayer"),
        startTime(0),
        a(nullptr),
        playing(false),
        didit(false) {}

  ~TestAudioPlayer() { delete a; }

  void update() {
    double time = getTime();
    if (time > startTime + 2 && !playing) {
      a->togglePause();
      playing = true;
      startTime = time;
    }

    if (time > startTime + 6 && !didit) {
      a->addPlaylist("res/playlist.txt");
      didit = true;
    }
  }

  void init() {
    startTime = getTime();
    MainCanvas *c = currentTab()->getMainCanvas();

    StyledMultiShape2D *m = c->getGui();

    a = new AudioPlayer();

    a->addFile("res/sample1mb.ogg");
    a->setVolume(75);
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestAudioPlayer(), 300, 300);
}

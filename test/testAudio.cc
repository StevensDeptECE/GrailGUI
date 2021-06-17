#include "audio/AudioPlayer.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestAudioPlayer : public GLWin {
 public:
  TestAudioPlayer() : GLWin(0x000000, 0xCCCCCC, "TestAudioPlayer") {}

  double startTime;
  AudioPlayer a;
  bool playing;

  void update() {
    if (getTime() > startTime + 3 && !playing) {
      a.togglePause();
      playing = true;
    }
  }

  void init() {
    startTime = getTime();
    playing = false;

    MainCanvas *c = currentTab()->getMainCanvas();

    StyledMultiShape2D *m = c->getGui();

    a = AudioPlayer();

    a.setFilePath("res/sample1mb.ogg");
    a.setVolume(50);
    a.init();
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestAudioPlayer(), 300, 300);
}

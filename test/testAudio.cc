#include "audio/AudioPlayer.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestAudioPlayer : public GLWin {
 private:
  double startTime;
  AudioPlayer *a;
  int step;

 public:
  TestAudioPlayer()
      : GLWin(0x000000, 0xCCCCCC, "TestAudioPlayer"),
        startTime(0),
        a(nullptr),
        step(0) {}

  ~TestAudioPlayer() { delete a; }

  void update() {
    double time = getTime();
    // printf("current time: %f\n", time);
    // printf("startTime: %f\n", startTime);
    if (time > startTime + 1 && step == 0) {
      a->setCurrentContext(0);
      a->togglePause();
      a->setCurrentContext(1);
      a->togglePause();
      startTime = time;
      step++;
    }

    if (time > startTime + 15 && step == 1) {
      a->setCurrentContext(0);
      a->togglePause();
      startTime = time;
      step++;
    }
  }

  void init() {
    startTime = getTime();
    MainCanvas *c = currentTab()->getMainCanvas();

    StyledMultiShape2D *m = c->getGui();

    a = new AudioPlayer();

    a->setCurrentContext(0);
    a->setVolume(75);
    a->addFile("res/sample1mb.ogg");

    a->newContext();
    a->setCurrentContext(1);
    a->setVolume(50);
    a->addPlaylist("res/playlist.txt");
  }
};

int main(int argc, char *argv[]) {
  return GLWin::init(new TestAudioPlayer(), 300, 300);
}

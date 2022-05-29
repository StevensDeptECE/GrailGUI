#include "audio/AudioPlayer.hh"
#include "opengl/GrailGUI.hh"

class TestSingleAudio : public Member {
 public:
  AudioPlayer *a;
  TestSingleAudio(GLWin *w) : Member(w) {
    a = new AudioPlayer();

    a->setCurrentContext("default");
    a->setVolume(50);
    a->loadFile("res/sample1mb.ogg");
    a->setPlaying();
  }
};

void grailmain(int argc, char *argv[], GLWin *w) { new TestSingleAudio(w); }
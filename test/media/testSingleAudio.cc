#include "audio/AudioPlayer.hh"
#include "opengl/GrailGUI.hh"

class TestSingleAudio : public Member {
 public:
  AudioPlayer *a;
  TestSingleAudio(Tab *tab) : Member(tab) {
    a = new AudioPlayer();

    a->setCurrentContext("default");
    a->setVolume(50);
    a->loadFile("res/sample1mb.ogg");
    a->setPlaying();
  }
};

void grailmain(int argc, char *argv[], GLWin *w, Tab *defaultTab) {
  defaultTab->addMember(new TestSingleAudio(defaultTab));
}
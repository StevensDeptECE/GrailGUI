#include "audio/AudioPlayer.hh"
#include "opengl/Animated.hh"
#include "opengl/GrailGUI.hh"

class TestSingleAudio : public Animated {
 public:
  AudioPlayer *a;
  TestSingleAudio(Tab *tab) : Animated(tab) {
    a = new AudioPlayer();

    a->setCurrentContext("default");
    a->setVolume(50);
    a->loadFile("res/sample1mb.ogg");
    a->setPlaying();
  }
};

void grailmain(int argc, char *argv[], GLWin *w, Tab *defaultTab) {
  defaultTab->addAnimated(new TestSingleAudio(defaultTab));
}
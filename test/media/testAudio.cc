#include "audio/AudioPlayer.hh"
#include "opengl/GrailGUI.hh"

using namespace std;
using namespace grail;

class TestAudioPlayer : public Member {
 private:
  double startTime;
  double time;
  AudioPlayer *a;
  int step;

  inline void helper(double elapsedTime, int step, string printOut,
                     void (*fnptr)(AudioPlayer *)) {
    if (time > startTime + elapsedTime && this->step == step) {
      printf("%s\n", printOut.c_str());
      (*fnptr)(a);
      startTime = time;
      this->step++;
    }
  }

 public:
  TestAudioPlayer(GLWin *w) : Member(w, 0), startTime(0), a(nullptr), step(0) {}

  // required to ensure that the memory of the audio player is freed
  ~TestAudioPlayer() { delete a; }

  // using the built in timing of GLWin and if statements allows for control
  // flow of the player with respect to time the program has been running
  void update() {
    time = tab->getTime();

    helper(1, 0, "simultaneous playback", [](AudioPlayer *a) {
      a->setCurrentContext("default");
      a->setPlaying();
      a->setCurrentContext("new context");
      a->setPlaying();
    });

    helper(10, 1, "play from youtube", [](AudioPlayer *a) {
      a->setCurrentContext("default");
      a->setPaused();
      a->setCurrentContext("new context");
      a->setPaused();

      a->setCurrentContext("from youtube");
      a->setPlaying();
    });

    helper(5, 2, "skip to 1:52 in song", [](AudioPlayer *a) {
      a->setCurrentContext("from youtube");
      a->seekLocation("1:52", "absolute");
      a->printCurrentTime();

      // this should fail, chicken not valid lmao
      // a->seekLocation("66", "chicken");
    });

    helper(7, 3, "revert skip", [](AudioPlayer *a) {
      a->setCurrentContext("from youtube");
      a->revertSeek();
      a->printCurrentTime();
    });

    helper(5, 4, "try to play a playlist from youtube", [](AudioPlayer *a) {
      a->setCurrentContext("from youtube");
      a->setPaused();

      a->setCurrentContext("skyhill");
      a->setPlaying();
    });

    helper(10, 5, "next song in playlist", [](AudioPlayer *a) {
      a->setCurrentContext("skyhill");
      // skip to next song in playlist
      a->playlistNext();
    });

    helper(10, 6, "go to specific index in playlist", [](AudioPlayer *a) {
      a->setCurrentContext("skyhill");
      // jump to a specific index in playlist
      a->playlistPlayIndex(6);
    });

    helper(7, 7, "go to prev track in playlist", [](AudioPlayer *a) {
      a->setCurrentContext("skyhill");
      // go back a track in playlist
      a->playlistPrev();
    });
  }

  void init() {
    startTime = tab->getTime();

    StyledMultiShape2D *m = c->getGui();

    a = new AudioPlayer();

    // The initial context created with an AudioPlayer is called default
    // Before performing operations on an mpv context, you have to set the
    // current context of the AudioPlayer object. Subsequent commands apply to
    // whatever context was set
    a->setCurrentContext("default");
    a->setVolume(50);
    a->loadFile("res/sample1mb.ogg");

    // creating a new context, calling it "new context"
    // set the current context to the new one, give it a playlist that mpv can
    // understand (see playlist.txt in test/res/)
    /* a->newContext("new context");
     a->setCurrentContext("new context");
     a->setVolume(50);
     a->loadPlaylist("res/playlist.txt");*/

    // if you try to set a current context that doesn't exist, a message will
    // print telling you what's happened
    a->setCurrentContext("i should print something");

    // mpv can play things from the internet utilizing youtube-dl, here's an
    // example of how to do it
    a->newContext("from youtube");
    a->setCurrentContext("from youtube");
    a->loadFile("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
    a->setVolume(50);

    // TODO: document this
    a->newContext("skyhill");
    a->setCurrentContext("skyhill");
    a->loadFile(
        "https://www.youtube.com/"
        "playlist?list=PLcKMZE_1oNKHZ5tN8FPiQNkLVTa-LrMpr");
    a->setVolume(50);
  }
};

void grailmain(int argc, char *argv[], GLWin *w) { new TestAudioPlayer(w); }

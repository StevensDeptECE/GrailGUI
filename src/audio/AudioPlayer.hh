#pragma once

#include <mpv/client.h>

#include <string>
#include <unordered_map>

#include "util/Ex.hh"

class AudioPlayer {
 private:
  struct context {
    std::string name;
    mpv_handle *handle;
    bool playing;
  };

  std::unordered_map<std::string, context> contexts;
  struct context *currentCtx;

  inline void checkError(int status) {
    if (status < 0) {
      printf("mpv API error: %s\n", mpv_error_string(status));
      throw Ex1(Errcode::MPV_FAILURE);
    }
  }

 public:
  AudioPlayer();
  ~AudioPlayer();

  AudioPlayer(const AudioPlayer &orig) = delete;
  AudioPlayer &operator=(const AudioPlayer &orig) = delete;

  void newContext(std::string name);
  void setCurrentContext(std::string name);

  void loadFile(std::string filePath);
  void loadPlaylist(std::string filePath, bool append = false);

  void setVolume(int volume);
  void seekLocation(std::string time, std::string type = "relative");
  void revertSeek();

  void playlistNext();
  void playlistPrev();
  void playlistPlayIndex(int index);
  void playlistRemoveIndex(int index);
  void playlistMove(int index1, int index2);
  void playlistShuffle();
  void playlistClear();

  void printCurrentTime();

  void setPaused();
  void setPlaying();
  void togglePause();
};

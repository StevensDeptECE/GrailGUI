#pragma once

#include <mpv/client.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>
#include <vector>

#include "util/Ex.hh"

class AudioPlayer {
 private:
  std::vector<mpv_handle *> contexts;
  mpv_handle *currentCtx;

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
  AudioPlayer& operator=(const AudioPlayer &orig) = delete;

  void addContext();
  void setCurrentContext(int index);
  void addFile(std::string filePath);
  void addPlaylist(std::string filePath, bool append = false);
  // void setFilePath(std::string filePath);
  // needs to check if between 0 and 100 (or a reasonable bound)
  void setVolume(int volume);
  void next();
  void init();
  void togglePause();
  static void classCleanup();
};

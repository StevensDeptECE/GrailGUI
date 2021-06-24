#include "audio/AudioPlayer.hh"

using namespace std;
AudioPlayer::AudioPlayer() { newContext("default"); }

AudioPlayer::~AudioPlayer() {
  for (auto pair = contexts.begin(); pair != contexts.end(); pair++) {
    mpv_terminate_destroy(pair->second);
  }

  // c++ 17 feature
  // for (const auto &[name, ctx] : contexts) {
  //   mpv_terminate_destroy(ctx);
  // }
}

void AudioPlayer::newContext(string name) {
  mpv_handle *ctx = mpv_create();

  if (!ctx) {
    printf("failed creating mpv context\n");
    throw Ex1(Errcode::MPV_FAILURE);
  }

  checkError(mpv_initialize(ctx));
  checkError(mpv_command_string(ctx, "cycle pause"));
  const char *cmd[] = {"set", "video", "no", nullptr};
  checkError(mpv_command(ctx, cmd));
  contexts[name] = ctx;
}

void AudioPlayer::setCurrentContext(string name) {
  auto pair = contexts.find(name);
  if (pair != contexts.end()) {
    currentCtx = pair->second;
  } else {
    printf("Couldn't set a context with name: %s\n", name.c_str());
  }
}

void AudioPlayer::addFile(string filePath) {
  const char *cmd[] = {"loadfile", filePath.c_str(), "append", nullptr};
  checkError(mpv_command(currentCtx, cmd));
}

void AudioPlayer::addPlaylist(string filePath, bool append) {
  const char *cmd[] = {"loadlist", filePath.c_str(),
                       (append) ? "append" : "replace", nullptr};
  checkError(mpv_command(currentCtx, cmd));
}

void AudioPlayer::setVolume(int volume) {
  if (volume < 1000 && volume >= 0) {
    char cmd[25];
    sprintf(cmd, "set volume %d", volume);
    checkError(mpv_command_string(currentCtx, cmd));
  } else {
    printf("Please provide a volume that is between 0 and 999\n");
  }
}

void AudioPlayer::seekLocation(string time, string type) {
  if (type == "relative" || type == "absolute" || type == "relative-percent" ||
      type == "absolute-percent") {
    const char *cmd[] = {"seek", time.c_str(), type.c_str(), nullptr};
    checkError(mpv_command(currentCtx, cmd));
  } else {
    printf(
        "Please provide one of the following for seek type: \nrelative, "
        "absolute, relative-percent, absolute-percent\n");
  }
}

void AudioPlayer::nextTrack() {
  const char *cmd[] = {"playlist-next", nullptr};
  checkError(mpv_command(currentCtx, cmd));
}

void AudioPlayer::togglePause() {
  checkError(mpv_command_string(currentCtx, "cycle pause"));
  isPlaying = !isPlaying;
}

void AudioPlayer::setPlaying() {
  if (!isPlaying) checkError(mpv_command_string(currentCtx, "cycle pause"));
}

void AudioPlayer::setPaused() {
  if (isPlaying) checkError(mpv_command_string(currentCtx, "cycle pause"));
}

#include "audio/AudioPlayer.hh"

using namespace std;
AudioPlayer::AudioPlayer() { newContext(); }

AudioPlayer::~AudioPlayer() {
  for (auto ctx : contexts) mpv_terminate_destroy(ctx);
}

void AudioPlayer::newContext() {
  mpv_handle *ctx = mpv_create();

  if (!ctx) {
    printf("failed creating mpv context\n");
    throw Ex1(Errcode::MPV_FAILURE);
  }

  checkError(mpv_initialize(ctx));
  checkError(mpv_command_string(ctx, "cycle pause"));
  contexts.push_back(ctx);
}

void AudioPlayer::setCurrentContext(int index) {
  if (index > contexts.size() - 1) {
    printf("Can't do that chief\n");
    return;
  }

  currentCtx = contexts[index];
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
  char cmd[25];
  sprintf(cmd, "set volume %d", volume);
  checkError(mpv_command_string(currentCtx, cmd));
}

void AudioPlayer::nextTrack() {
  const char *cmd[] = {"playlist-next", nullptr};
  checkError(mpv_command(currentCtx, cmd));
}

void AudioPlayer::togglePause() {
  checkError(mpv_command_string(currentCtx, "cycle pause"));
}

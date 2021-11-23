#include "audio/AudioPlayer.hh"

using namespace std;
AudioPlayer::AudioPlayer() {
  newContext("default");
  setCurrentContext("default");
}

AudioPlayer::~AudioPlayer() {
  // fun little c++ 17 feature
  for (const auto &[name, context] : contexts) {
    mpv_terminate_destroy(context.handle);
  }

  printf("audio has terminated successfully\n");
}

void AudioPlayer::newContext(string name) {
  mpv_handle *mpv = mpv_create();

  if (!mpv) {
    printf("failed creating mpv context\n");
    throw Ex1(Errcode::MPV_FAILURE);
  }

  checkError(mpv_initialize(mpv));

  const char *cmd[] = {"set", "video", "no", nullptr};
  checkError(mpv_command(mpv, cmd));

  const char *cmd_pause[] = {"cycle", "pause", nullptr};
  checkError(mpv_command(mpv, cmd_pause));

  struct context ctx = {
      .name = name,
      .handle = mpv,
      .playing = false,
  };

  contexts[name] = ctx;
}

void AudioPlayer::setCurrentContext(string name) {
  auto pair = contexts.find(name);
  if (pair != contexts.end()) {
    currentCtx = &pair->second;
  } else {
    printf("Couldn't set a context with name: %s\n", name.c_str());
  }
}

void AudioPlayer::loadFile(string filePath) {
  const char *cmd[] = {"loadfile", filePath.c_str(), "append", nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::loadPlaylist(string filePath, bool append) {
  const char *cmd[] = {"loadlist", filePath.c_str(),
                       (append) ? "append" : "replace", nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::setVolume(int volume) {
  char intString[33];
  sprintf(intString, "%d", volume);
  const char *cmd[] = {"set", "volume", intString, nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::seekLocation(string time, string type) {
  if (type == "relative" || type == "absolute" || type == "relative-percent" ||
      type == "absolute-percent") {
    const char *cmd[] = {"seek", time.c_str(), type.c_str(), nullptr};
    checkError(mpv_command(currentCtx->handle, cmd));
  } else {
    printf(
        "Please provide one of the following for seek type: \nrelative, "
        "absolute, relative-percent, absolute-percent\n");
  }
}

void AudioPlayer::revertSeek() {
  const char *cmd[] = {"revert-seek", nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::playlistNext() {
  const char *cmd[] = {"playlist-next", nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::playlistPrev() {
  const char *cmd[] = {"playlist-prev", nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::playlistPlayIndex(int index) {
  char indexString[33];
  sprintf(indexString, "%d", index);
  const char *cmd[] = {"set", "playlist-pos", indexString, nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::playlistClear() {
  const char *cmd[] = {"playlist-clear", nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::playlistRemoveIndex(int index) {
  char indexString[33];
  sprintf(indexString, "%d", index);
  const char *cmd[] = {"playlist-remove", indexString, nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::playlistMove(int index1, int index2) {
  char index1String[33];
  char index2String[33];
  sprintf(index1String, "%d", index1);
  sprintf(index2String, "%d", index2);
  const char *cmd[] = {"playlist-move", index1String, index2String, nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::playlistShuffle() {
  const char *cmd[] = {"playlist-shuffle", nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
}

void AudioPlayer::printCurrentTime() {
  mpv_node result;

  checkError(mpv_get_property(currentCtx->handle, "time-pos", MPV_FORMAT_NODE,
                              &result));
  printf("Track Time Elapsed in Seconds: %.2f\n", result.u.double_);
  mpv_free_node_contents(&result);
}

void AudioPlayer::setPaused() {
  printf("%s old playing val: %d\n", currentCtx->name.c_str(),
         currentCtx->playing);
  if (currentCtx->playing == true) {
    togglePause();
  }
  printf("%s new playing val: %d\n", currentCtx->name.c_str(),
         currentCtx->playing);
}

void AudioPlayer::setPlaying() {
  printf("%s old playing val: %d\n", currentCtx->name.c_str(),
         currentCtx->playing);
  if (currentCtx->playing == false) {
    togglePause();
  }
  printf("%s new playing val: %d\n", currentCtx->name.c_str(),
         currentCtx->playing);
}

void AudioPlayer::togglePause() {
  const char *cmd[] = {"cycle", "pause", nullptr};
  checkError(mpv_command(currentCtx->handle, cmd));
  currentCtx->playing = !currentCtx->playing;
}

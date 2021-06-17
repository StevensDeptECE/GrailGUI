#include "AudioPlayer.hh"

using namespace std;
AudioPlayer::AudioPlayer(string filePath, int volume)
    : filePath(filePath), volume(volume) {}

void AudioPlayer::setFilePath(string filePath) { this->filePath = filePath; }

void AudioPlayer::setVolume(int volume) { this->volume = volume; }

void AudioPlayer::init() {
  ctx = mpv_create();
  if (!ctx) {
    printf("failed creating context\n");
    // todo, crash / fail here
  }

  check_error(mpv_initialize(ctx));
  check_error(mpv_command_string(ctx, "cycle pause"));

  const char *loadCmd[] = {"loadfile", filePath.c_str(), NULL};
  check_error(mpv_command(ctx, loadCmd));

  char volumeCmd[25];
  sprintf(volumeCmd, "set volume %d", volume);
  check_error(mpv_command_string(ctx, volumeCmd));
}

void AudioPlayer::togglePause() {
  check_error(mpv_command_string(ctx, "cycle pause"));
}

void AudioPlayer::destroy() { mpv_terminate_destroy(ctx); }

#include <mpv/client.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>

class AudioPlayer {
 private:
  std::string filePath;
  int volume;
  mpv_handle *ctx;

  static inline void check_error(int status) {
    if (status < 0) {
      printf("mpv API error: %s\n", mpv_error_string(status));
      exit(1);
    }
  }

 public:
  AudioPlayer(std::string filePath = "", int volume = 75);
  void setFilePath(std::string filePath);
  // needs to check if between 0 and 100 (or a reasonable bound)
  void setVolume(int volume);
  void init();
  void togglePause();
  void destroy();
};

#if 0
int main(int argc, char *argv[]) {
  const char *vol;

  if (argc < 3) {
    vol = "50";
  } else {
    vol = argv[2];
  }

  if (argc < 2) {
    printf("pass a single media file as argument\n");
    return 1;
  }

  // Enable default key bindings, so the user can actually interact with
  // the player (and e.g. close the window).
  // check_error(mpv_set_option_string(ctx, "input-default-bindings", "yes"));
  // mpv_set_option_string(ctx, "input-vo-keyboard", "yes");
  // int val = 1;
  // check_error(mpv_set_option(ctx, "osc", MPV_FORMAT_FLAG, &val));

  // Done setting up options.
  // check_error(mpv_initialize(ctx));

  // Play this file.
  // const char *cmd[] = {"loadfile", argv[1], NULL};
  // check_error(mpv_command(ctx, cmd));

  // char volcmd[20];
  // sprintf(volcmd, "set volume %s", vol);
  // check_error(mpv_command_string(ctx, volcmd));

  // Let it play, and wait until the user quits.
  // while (1) {
  //   mpv_event *event = mpv_wait_event(ctx, 10000);
  //   printf("event: %s\n", mpv_event_name(event->event_id));
  //   if (event->event_id == MPV_EVENT_SHUTDOWN) break;
  // }

  mpv_terminate_destroy(ctx);
  return 0;
}
#endif

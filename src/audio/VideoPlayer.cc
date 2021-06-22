#include "audio/VideoPlayer.hh"

extern "C" {
static void *get_proc_address_mpv(void *fn_ctx, const char *name) {
  return (void *)glfwGetProcAddress(name);
}
static void on_mpv_events(void *ctx) { glfwPostEmptyEvent(); }

static void on_mpv_render_update(void *ctx) { glfwPostEmptyEvent(); }
}

VideoPlayer::VideoPlayer() {
  advancedControl = 1;
  mpv = mpv_create();
  if (!mpv) {
    printf("failed creating mpv context\n");
    throw Ex1(Errcode::MPV_FAILURE);
  }
  checkError(mpv_initialize(mpv));
  // checkError(mpv_initialize(mpv));
  mpv_request_log_messages(mpv, "debug");

  auto renderParam = (mpv_opengl_init_params){
      .get_proc_address = get_proc_address_mpv,
  };

  mpv_render_param params[] = {
      {MPV_RENDER_PARAM_API_TYPE, (void *)MPV_RENDER_API_TYPE_OPENGL},
      {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &renderParam},
      {MPV_RENDER_PARAM_ADVANCED_CONTROL, &advancedControl},
      (mpv_render_param_type)0};

  checkError(mpv_render_context_create(&mpv_gl, mpv, params));

  mpv_set_wakeup_callback(mpv, on_mpv_events, nullptr);
  mpv_render_context_set_update_callback(mpv_gl, on_mpv_render_update, nullptr);
}

void VideoPlayer::setVid(std::string filePath) {
  checkError(mpv_command_string(mpv, "cycle pause"));
  const char *cmd[] = {"loadfile", filePath.c_str(), nullptr};
  mpv_command_async(mpv, 0, cmd);
}

void VideoPlayer::togglePause() {
  checkError(mpv_command_string(mpv, "cycle pause"));
}

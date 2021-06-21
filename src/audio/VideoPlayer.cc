#include "audio/VideoPlayer.hh"
#include "opengl/GLWin.hh"

VideoPlayer::VideoPlayer() {
  mpv = mpv_create();
  if (!mpv) {
    printf("failed creating mpv context\n");
    throw Ex1(Errcode::MPV_FAILURE);
  }
  checkError(mpv_initialize(mpv));
  // checkError(mpv_initialize(mpv));
  mpv_request_log_messages(mpv, "debug");

  auto thing = (mpv_opengl_init_params){
      .get_proc_address = [](void *fn_ctx, const char *name) -> void * {
        return (void *)glfwGetProcAddress(name);
      },
  };

  mpv_render_param params[] = {
      {MPV_RENDER_PARAM_API_TYPE, (void *)MPV_RENDER_API_TYPE_OPENGL},
      {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &thing},
      {MPV_RENDER_PARAM_ADVANCED_CONTROL, (int *)1},
      (mpv_render_param_type)0};

  checkError(mpv_render_context_create(&mpv_gl, mpv, params));

  mpv_set_wakeup_callback(
      mpv, [](void *ctx) { glfwPostEmptyEvent(); }, nullptr);
  mpv_render_context_set_update_callback(
      mpv_gl, [](void *ctx) { glfwPostEmptyEvent(); }, nullptr);
}

void VideoPlayer::setVid(std::string filePath) {
  checkError(mpv_command_string(mpv, "cycle pause"));
  const char *cmd[] = {"loadfile", filePath.c_str(), NULL};
  mpv_command_async(mpv, 0, cmd);
}

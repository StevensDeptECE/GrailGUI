#include "video/VideoPlayer.hh"

#include "opengl/Canvas.hh"

extern "C" {
static void *get_proc_address_mpv(void *fn_ctx, const char *name) {
  return (void *)glfwGetProcAddress(name);
}
static void on_mpv_events(void *ctx) { glfwPostEmptyEvent(); }

static void on_mpv_render_update(void *ctx) { glfwPostEmptyEvent(); }
}

VideoPlayer::VideoPlayer(Canvas *c, float x, float y, int width, int height)
    : Shape(c),
      x(x),
      y(y),
      drawWidth(width),
      drawHeight(height),
      flipY(0),
      advancedControl(1) {
  mpv = mpv_create();
  if (!mpv) {
    printf("failed creating mpv context\n");
    throw Ex1(Errcode::MPV_FAILURE);
  }

  checkError(mpv_initialize(mpv));
  mpv_request_log_messages(mpv, "debug");

  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, drawWidth, drawHeight, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture, 0);

  glBindTexture(GL_TEXTURE_2D, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  fboParams = {
      .fbo = (int)fbo,
      .w = width,
      .h = height,
  };

  openglInitParams = {
      .get_proc_address = get_proc_address_mpv,
  };

  initialRenderParams[0] = {MPV_RENDER_PARAM_API_TYPE,
                            (void *)MPV_RENDER_API_TYPE_OPENGL};
  initialRenderParams[1] = {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS,
                            &openglInitParams};
  initialRenderParams[2] = {MPV_RENDER_PARAM_ADVANCED_CONTROL,
                            &advancedControl};
  initialRenderParams[3] = {MPV_RENDER_PARAM_INVALID, nullptr};

  redrawParams[0] = {MPV_RENDER_PARAM_OPENGL_FBO, &fboParams};
  // Flip rendering (needed due to flipped GL coordinate system).
  redrawParams[1] = {MPV_RENDER_PARAM_FLIP_Y, &flipY};
  redrawParams[2] = {MPV_RENDER_PARAM_INVALID, nullptr};

  checkError(mpv_render_context_create(&mpv_gl, mpv, initialRenderParams));
  mpv_set_wakeup_callback(mpv, on_mpv_events, nullptr);
  mpv_render_context_set_update_callback(mpv_gl, on_mpv_render_update, nullptr);
}

VideoPlayer::~VideoPlayer() { glDeleteFramebuffers(1, &fbo); }

void VideoPlayer::render() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
              << std::endl;

  checkError(mpv_render_context_render(mpv_gl, redrawParams));

  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

  glBlitFramebuffer(0, 0, drawWidth, drawHeight, x, 1000 - y, x + drawWidth,
                    1000 - (y + drawHeight), GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void VideoPlayer::update() {}

void VideoPlayer::init() {}

void VideoPlayer::setVid(std::string filePath) {
  checkError(mpv_command_string(mpv, "cycle pause"));
  const char *cmd[] = {"loadfile", filePath.c_str(), nullptr};
  mpv_command_async(mpv, 0, cmd);
}

void VideoPlayer::togglePause() {
  checkError(mpv_command_string(mpv, "cycle pause"));
}

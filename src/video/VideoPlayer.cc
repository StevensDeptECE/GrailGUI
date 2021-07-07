#include "video/VideoPlayer.hh"

#include "opengl/Canvas.hh"

using namespace std;

static void *get_proc_address_mpv(void *fn_ctx, const char *name) {
  return (void *)glfwGetProcAddress(name);
}
static void on_mpv_events(void *ctx) { glfwPostEmptyEvent(); }

static void on_mpv_render_update(void *ctx) { glfwPostEmptyEvent(); }

VideoPlayer::VideoPlayer(Canvas *c, float x, float y, int width, int height)
    : Shape(c),
      x(x),
      y(y),
      width(width),
      height(height),
      advancedControl(1),
      xLeft(0),
      xRight(0),
      yTop(0),
      yBottom(0) {
  // create a standard mpv handle
  mpv = mpv_create();
  // if we failed to make one, die
  if (!mpv) {
    printf("failed creating mpv context\n");
    throw Ex1(Errcode::MPV_FAILURE);
  }

  // try to initialize the mpv handle
  // utilize checkError to die if this fails
  checkError(mpv_initialize(mpv));
  // try to request log messages
  // die if this fails
  checkError(mpv_request_log_messages(mpv, "debug"));

  // create 1 opengl framebuffer, stick a way name it in the fbo variable
  glGenFramebuffers(1, &fbo);
  // opengl operates based on binding to a particular thing, then operating on
  // it with future commands before binding to something else
  // in this case, bind to the framebuffer we just made so that future commands
  // apply to it
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  // create one opengl texture, stick a way to name it in the texture variable
  glGenTextures(1, &texture);
  // similarly to binding to the framebuffer, bind to the newly created texture
  glBindTexture(GL_TEXTURE_2D, texture);
  // allocate memory for the texture of a width and height that the user has
  // provided, but don't put anything in there, hence the nullptr at the end
  // (mpv will put data in here for us later)
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, nullptr);
  // set some parameters for the texture, from what I understand these relate to
  // how it will scale by getting bigger or smaller (linear is a way to scale
  // images)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // attach the created texture to the created frame buffer so that it can be
  // utilized later on by mpv, if this attachment wasn't done, the framebuffer
  // would be considered "incomplete" and things would go south quickly
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture, 0);

  // now that we've created our new things, we want to return the state of
  // opengl to how it was before this, so we set the currently bound texture and
  // framebuffer to be 0, or the defaults in case anything else in the future
  // wants to do things with opengl and doesn't ensure to bind to the desired
  // framebuffer and or texture
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // a struct to be used by mpv later, it will tell mpv what framebuffer to
  // render into, and the width and height of what we should draw there
  fboParams = {
      .fbo = (int)fbo,
      .w = width,
      .h = height,
  };

  // a struct used by mpv later, it will tell mpv what function it can call to
  // get the proc address of the opengl instance created by glfw
  openglInitParams = {
      .get_proc_address = get_proc_address_mpv,
  };

  // tells mpv that we are using opengl as the render backend
  initialRenderParams[0] = {MPV_RENDER_PARAM_API_TYPE,
                            (void *)MPV_RENDER_API_TYPE_OPENGL};
  // pass in opengl init params
  initialRenderParams[1] = {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS,
                            &openglInitParams};
  // tell mpv we want advanced control (not 100 percent sure this is needed, but
  // in mpv's examples they used it )
  initialRenderParams[2] = {MPV_RENDER_PARAM_ADVANCED_CONTROL,
                            &advancedControl};
  // tells mpv that this is the end of the list of init params
  initialRenderParams[3] = {MPV_RENDER_PARAM_INVALID, nullptr};

  // tells what parameters we want regarding drawing to an fbo
  redrawParams[0] = {MPV_RENDER_PARAM_OPENGL_FBO, &fboParams};
  // tells mpv this is the end of the list of render params
  redrawParams[1] = {MPV_RENDER_PARAM_INVALID, nullptr};

  // try to create an mpv render handle, putting the handle at mpv_gl, passing
  // it the previously created mpv context, and the list of initial parameters
  checkError(mpv_render_context_create(&mpv_gl, mpv, initialRenderParams));

  // set the callback function for what mpv should call when it recieves an
  // event of its own
  // generally we aren't looking for mpv events, so this should never be run
  // (mpv_wait_event() is never called by us)
  mpv_set_wakeup_callback(mpv, on_mpv_events, nullptr);
  // set the callback function for what mpv should call when a new video frame
  // is available
  mpv_render_context_set_update_callback(mpv_gl, on_mpv_render_update, nullptr);
}

VideoPlayer::~VideoPlayer() {
  // destroy the framebuffer we created for mpv
  glDeleteFramebuffers(1, &fbo);
  // destroy the texture we attached to the framebuffer for mpv
  glDeleteTextures(1, &texture);
  // destroy the GL renderer and all of the GL objects it allocated. If video
  // is still running, the video track will be deselected.
  mpv_render_context_free(mpv_gl);
  // destroy the mpv handle
  mpv_detach_destroy(mpv);
  // indictes that we've cleaned up everything that VideoPlayer created
  printf("videoplayer has terminated succesfully");
}

void VideoPlayer::init() {}

void VideoPlayer::update() {
  uint64_t flags = mpv_render_context_update(mpv_gl);
  if (flags & MPV_RENDER_UPDATE_FRAME) parentCanvas->getWin()->setDirty();
}

void VideoPlayer::render() {
  // bind the current framebuffer to our created fbo
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  // if the framebuffer somehow isn't complete, throw an exception, the program
  // shouldn't proceed
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf(
        "Framebuffer created for MPV is not complete, welcome to undefined "
        "behavior\n");
    throw Ex1(Errcode::MPV_FAILURE);
  }

  // try to render the current video frame into the fbo
  // if we fail, die
  checkError(mpv_render_context_render(mpv_gl, redrawParams));

  // glBindFramebuffer can take 1 of three values, GL_FRAMEBUFFER,
  // GL_DRAW_FRAMEBUFFER, and GL_READ_FRAMEBUFFER
  // GL_DRAW is the buffer we will draw to for functions that draw somewhere,
  // GL_READ is the buffer we read from for functions that read from framebuffer
  // GL_FRAMEBUFFER binds the supplied fbo to both
  // here we set the draw buffer to the default, and the read to the fbo we gave
  // mpv
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

  // blitting is basically just copying pixel data from one location to another
  // here we copy the data from (0, 0) to (drawWidth, drawheight) of the fbo
  // that mpv put data into (this should be the entire thing), over to the
  // default framebuffer at the weird coordinates below
  //
  // these weird coordinates are a correction of mpv assuming that 0, 0 is the
  // bottom left hand corner of the screen, whereas the rest of grail considers
  // the top left hand corner of the screen to be (0, 0) and positive y going
  // upwards, and positive y to be going downwards
  glBlitFramebuffer(xLeft, yBottom, width - xRight, height - yTop, x,
                    parentCanvas->getHeight() - y, x + width,
                    parentCanvas->getHeight() - (y + height),
                    GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void VideoPlayer::loadFile(string filePath) {
  const char *cmd[] = {"loadfile", filePath.c_str(), nullptr};
  checkError(mpv_command_async(mpv, 0, cmd));
  setPaused();
}

void VideoPlayer::loadPlaylist(string filePath, bool append) {
  const char *cmd[] = {"loadlist", filePath.c_str(),
                       (append) ? "append" : "replace", nullptr};
  checkError(mpv_command_async(mpv, 0, cmd));
}

void VideoPlayer::setVolume(int volume) {
  char intString[33];
  sprintf(intString, "%d", volume);
  const char *cmd[] = {"set", "volume", intString, nullptr};
  checkError(mpv_command_async(mpv, 0, cmd));
}

void VideoPlayer::seekLocation(string time, string type) {
  if (type == "relative" || type == "absolute" || type == "relative-percent" ||
      type == "absolute-percent") {
    const char *cmd[] = {"seek", time.c_str(), type.c_str(), nullptr};
    checkError(mpv_command_async(mpv, 0, cmd));
  } else {
    printf(
        "Please provide one of the following for seek type: \nrelative, "
        "absolute, relative-percent, absolute-percent\n");
  }
}

void VideoPlayer::cropVideo(float xLeft, float xRight, float yTop,
                            float yBottom) {
  this->xLeft = xLeft;
  this->xRight = xRight;
  this->yBottom = yBottom;
  this->yTop = yTop;
}

void VideoPlayer::setPaused() {
  if (isPlaying) {
    const char *cmd[] = {"cycle", "pause", nullptr};
    checkError(mpv_command_async(mpv, 0, cmd));
    isPlaying = !isPlaying;
  }
}

void VideoPlayer::setPlaying() {
  if (!isPlaying) {
    const char *cmd[] = {"cycle", "pause", nullptr};
    checkError(mpv_command_async(mpv, 0, cmd));
    isPlaying = !isPlaying;
  }
}

void VideoPlayer::togglePause() {
  const char *cmd[] = {"cycle", "pause", nullptr};
  checkError(mpv_command_async(mpv, 0, cmd));
}

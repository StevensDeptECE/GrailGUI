#pragma once

#include <glad/glad.h>

// This comment is keeping the peace between glad and glfw
// Removing glad causes some errors at compile-time
// Refer to GLWin.cc for a similar situation
#include <GLFW/glfw3.h>
#include <mpv/client.h>
#include <mpv/render_gl.h>

#include "opengl/Shape.hh"
#include "util/Ex.hh"

class VideoPlayer : public Shape {
 private:
  int flipY, advancedControl;
  float winWidth, winHeight;
  float x, y, drawWidth, drawHeight;
  uint32_t fbo, texture;
  std::vector<float> vertices;
  std::vector<uint32_t> indices;

  mpv_handle *mpv;

  // struct with 3 internal bits
  mpv_opengl_fbo fboParams;
  // struct where we only end up setting one value
  mpv_opengl_init_params openglInitParams;

  mpv_render_param initialRenderParams[4];
  // list of lists, where each internal list contains an enum in the first slot,
  // and data in the second
  mpv_render_param redrawParams[3];

  inline void checkError(int status) {
    if (status < 0) {
      printf("mpv API error: %s\n", mpv_error_string(status));
      throw Ex1(Errcode::MPV_FAILURE);
    }
  }

 public:
  // TODO: this shouldn't be public.
  // currently it needs to be public so that a child of GLWin can use it in its
  // update function to determine whether or not to setDirty(). I want to do
  // that from within Video Player, but currently no way to do that exists.
  mpv_render_context *mpv_gl;

  VideoPlayer(Canvas *c, float x, float y, int width, int height );
  ~VideoPlayer();

  // get rid of the copy constructors
  VideoPlayer(const VideoPlayer &orig) = delete;
  VideoPlayer &operator=(const VideoPlayer &orig) = delete;

  void setVid(std::string filePath);
  void togglePause();

  void init();
  void update();
  void render();
};

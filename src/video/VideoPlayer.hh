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
  // c++ wont let you pass something like &4 as a parameter, so we create an
  // advanced control variable within the class to make sure there's a valid
  // address to point to
  int advancedControl;

  float x, y, drawWidth, drawHeight;
  uint32_t fbo, texture;
  std::vector<float> vertices;
  std::vector<uint32_t> indices;

  mpv_handle *mpv;

  // struct with 3 internal bits
  mpv_opengl_fbo fboParams;
  // struct where we only end up setting one value
  mpv_opengl_init_params openglInitParams;

  // a list of lists, where each iternal list contains an enum in the first slot
  // and some kind of data in the second slot
  // this will be passed once to mpv giving it initial parameters when creating
  // an mpv render context
  mpv_render_param initialRenderParams[4];
  // list of lists, where each internal list contains an enum in the first slot,
  // and data in the second
  // this gets passed to mpv each time the mpv render context redraws, so it's
  // values could change and affect the output of what shows up on screen
  mpv_render_param redrawParams[2];

  // most mpv functions that can fail return an integer, where 0 is success, and
  // anything else is some kind of error
  // this function checks whether a function returned an error code, and if so
  // prints to the screen what that error code means (from mpv_error_string),
  // then throws a general mpv exception, halting the program
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
  // update, a function now exists within canvas to get a pointer to the GLwin
  // instance, just need to implement it
  mpv_render_context *mpv_gl;

  VideoPlayer(Canvas *c, float x, float y, int width, int height);
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

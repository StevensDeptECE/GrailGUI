#pragma once

#include <glad/glad.h>

// This comment is keeping the peace between glad and glfw
// Removing glad causes some errors at compile-time
// Refer to GLWin.cc for a similar situation
#include <GLFW/glfw3.h>
#include <mpv/client.h>
#include <mpv/render_gl.h>

#include "opengl/GLWin.hh"
#include "util/Ex.hh"

class VideoPlayer {
 private:
  mpv_handle *mpv;

  inline void checkError(int status) {
    if (status < 0) {
      printf("mpv API error: %s\n", mpv_error_string(status));
      throw Ex1(Errcode::MPV_FAILURE);
    }
  }

  // void on_mpv_events(void *ctx) { glfwPostEmptyEvent(); }

  // void on_mpv_render_update(void *ctx) { glfwPostEmptyEvent(); }

 public:
  mpv_render_context *mpv_gl;
  VideoPlayer();
  // ~VideoPlayer();
  VideoPlayer(const VideoPlayer &orig) = delete;
  VideoPlayer &operator=(const VideoPlayer &orig) = delete;
  void setVid(std::string filePath);
};

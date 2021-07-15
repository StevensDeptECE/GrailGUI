#pragma once

#include <glad/glad.h>
// don't remove this or glad/glfw gets angry
#include <GLFW/glfw3.h>
#include <mpv/client.h>
#include <mpv/render_gl.h>

#include "opengl/Shape.hh"
#include "util/Ex.hh"

/**
 * @brief
 *
 */
class VideoPlayer : public Shape {
 private:
  bool isPlaying; /**< Whether the video is playing or not*/

  int advancedControl; /**< Whether to use advanced control with MPV (defaults
                        * to 1). C++ wont let you pass something like &4 as a
                        * parameter, so we create this varaible within the class
                        * so there's a valid memory address to point at.
                        */

  float x;      /**< X coord to place video */
  float y;      /**< Y coord to place video */
  float width;  /**< Width of the player */
  float height; /**< Height of the player */

  // how much to crop the drawn video by
  float xLeft;   /**< Number of pixels to crop from the left margin of the
                    original video */
  float xRight;  /**< Number of pixels to crop from the right margin of the
                    original video*/
  float yTop;    /**< Number of pixels to crop from the upper margin of the
                    original video*/
  float yBottom; /**< Number of pixels to crop from the lower margin of the
                    original video*/

  uint32_t
      fbo; /**< Contains an id to reference a framebuffer created by OpenGL */
  uint32_t
      texture; /**< Contains an id to reference a texture created by OpenGL */

  mpv_handle *mpv; /**< MPV context for loading and playing audio/video */

  mpv_render_context
      *mpv_gl; /**< MPV render context which renders frames into the fbo */

  mpv_opengl_fbo fboParams;                /**< */
  mpv_opengl_init_params openglInitParams; /**< */

  // a list of lists, where each iternal list contains an enum in the first slot
  // and some kind of data in the second slot
  // this will be passed once to mpv giving it initial parameters when creating
  // an mpv render context
  mpv_render_param initialRenderParams[4]; /**< */
  // list of lists, where each internal list contains an enum in the first slot,
  // and data in the second
  // this gets passed to mpv each time the mpv render context redraws, so it's
  // values could change and affect the output of what shows up on screen
  mpv_render_param redrawParams[2]; /**< */

  // most mpv functions that can fail return an integer, where 0 is success, and
  // anything else is some kind of error
  // this function checks whether a function returned an error code, and if so
  // prints to the screen what that error code means (from mpv_error_string),
  // then throws a general exception from grail, halting the program
  /**
   * @brief
   *
   * @param status
   */
  inline void checkError(int status) {
    if (status < 0) {
      printf("mpv API error: %s\n", mpv_error_string(status));
      throw Ex1(Errcode::MPV_FAILURE);
    }
  }

 public:
  /**
   * @brief Construct a new Video Player object
   *
   * @param c Pointer to a Canvas
   * @param x X coord to place video
   * @param y Y coord to place video
   * @param width Width of the player
   * @param height Height of the player
   */
  VideoPlayer(Canvas *c, float x, float y, int width, int height);

  /**
   * @brief Destroy the Video Player object
   *
   */
  ~VideoPlayer();

  // get rid of the copy constructors
  VideoPlayer(const VideoPlayer &orig) = delete;
  VideoPlayer &operator=(const VideoPlayer &orig) = delete;

  /**
   * @brief Artifact of being subclassed from Shape, currently does nothing.
   *
   */
  void init();

  /**
   * @brief Ask MPV if it has updates to display and set the current GLWin to be
   * dirty if so.
   *
   */
  void update();

  /**
   * @brief Render the current frame with the specified x, y, width, and height.
   *
   */
  void render();

  /**
   * @brief Load a video file from a file path.
   *
   * @param filePath Path to the file to load. This can also be a URL to a
   * website supported by youtube-dl, including playlists on YouTube
   */
  void loadFile(std::string filePath);

  /**
   * @brief Load multiple audio files from a playlist text file.
   * This does not work with playlists on the internet.
   *
   * @param filePath Path to the playlist text file.
   * @param append Whether to replace the current playlist or to append to the
   * end of it.
   */
  void loadPlaylist(std::string filePath, bool append = false);

  /**
   * @brief Set the volume
   *
   * @param volume Between 0 and 100
   */
  void setVolume(int volume);

  /**
   * @brief Seek to a location
   *
   * @param time Where to seek to. Has different results based on the type.
   * @param type Only succeeds with one of the following values:
   * "relative": Jumps forward or backward the given amount of time.
   * "absolute": Jumps to the timestamp given.
   * "relative-percent": Jumps forward or backward the given percent of the
   * file.
   * "absolute-percent": Jumps to the given percent through the file.
   */
  void seekLocation(std::string time, std::string type = "relative");

  /**
   * @brief Revert the previous seek operation.
   *
   */
  void revertSeek();

  /**
   * @brief Go to the next track in a playlist.
   *
   */
  void playlistNext();

  /**
   * @brief Go to the previous track in a playlist.
   *
   */
  void playlistPrev();

  /**
   * @brief Go to a track at a specific index of a playlist.
   *
   * @param index Must be a valid index of the playlist, or the program
   * will crash. This behavior will be changed in the future.
   */
  void playlistPlayIndex(int index);

  /**
   * @brief Remove the track at a specific index of a playlist.
   *
   * @param index Must be a valid index of the playlist, or the program will
   * crash. This behavior will be changed in the future.
   */
  void playlistRemoveIndex(int index);

  /**
   * @brief Switch the position of two tracks in a playlist.
   *
   * @param index1 index1 Must be a valid index of the playlist, or the program
   * will crash. This behavior will be changed in the future.
   * @param index2 See index1.
   */
  void playlistMove(int index1, int index2);

  /**
   * @brief Shuffle the current playlist. Clear the current playlist.
   *
   */
  void playlistShuffle();

  /**
   * @brief Clear the current playlist.
   *
   */
  void playlistClear();

  /**< Number of pixels to crop from the left margin of the original video */
  /**< Number of pixels to crop from the right margin of the original video */
  /**< Number of pixels to crop from the upper margin of the original video */
  /**< Number of pixels to crop from the lower margin of the original video */

  /**
   * @brief Crop the source video
   *
   * @param xLeft Number of pixels to crop from the left margin of the
   * original video
   * @param xRight Number of pixels to crop from the right margin of the
   * original video
   * @param yTop Number of pixels to crop from the upper margin of the
   * original video
   * @param yBottom Number of pixels to crop from the lower margin of the
   * original video
   */
  void cropVideo(float xLeft, float xRight, float yTop, float yBottom);

  /**
   * @brief Pause the player.
   *
   */
  void setPaused();

  /**
   * @brief Player the player.
   *
   */
  void setPlaying();

  /**
   * @brief Toggle the playing state of the player.
   *
   */
  void togglePause();
};

#pragma once

#include <mpv/client.h>

#include <string>
#include <unordered_map>

#include "util/Ex.hh"

/**
 * @brief
 *
 */
class AudioPlayer {
 private:
  /**
   * @brief Contains relevant information ascociated with generated MPV handles.
   *
   */
  struct context {
    std::string name;   /**< The name used when inserting this struct into the
                           contexts map */
    mpv_handle* handle; /**< Points to a MPV context */
    bool playing;       /**< Whether this context is currently playing audio */
  };

  std::unordered_map<std::string, context> contexts;
  struct context* currentCtx; /**< Points to the context in contexts that
                                 functions will operate on */

  /**
   * @brief Check whether an MPV function succeeded and kill the program if it
   * didn't.
   *
   * @param status Expected to the the return value of a MPV function
   */
  inline void checkError(int status) {
    if (status < 0) {
      printf("mpv API error: %s\n", mpv_error_string(status));
      throw Ex1(Errcode::MPV_FAILURE);
    }
  }

 public:
  /**
   * @brief Construct a new Audio Player object
   *
   */
  AudioPlayer();
  /**
   * @brief Destroy the Audio Player object
   *
   */
  ~AudioPlayer();

  AudioPlayer(const AudioPlayer& orig) = delete;
  AudioPlayer& operator=(const AudioPlayer& orig) = delete;

  /**
   * @brief Create a new MPV context, wrapping it in a context struct and
   * placing it within the contexts map. This will not change the current
   * context.
   *
   * @param name The name used to retrieve the created context in the future.
   */
  void newContext(std::string name);

  /**
   * @brief Set the currentCtx variable
   *
   * @param name The name of the context to retrieve. If this isn't valid a
   * message will print to stdout.
   */
  void setCurrentContext(std::string name);

  /**
   * @brief Load an audio file from a file path into the currentCtx.
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
   * @param append Whether to replace the current playlist of currentCtx, or to
   * append to the end of it
   */
  void loadPlaylist(std::string filePath, bool append = false);

  /**
   * @brief Set the volume of currentCtx
   *
   * @param volume
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
   * @param index Must be a valid index of the playlist, or the program will
   * crash. This behavior will be changed in the future.
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
   * @param index1 Must be a valid index of the playlist, or the program will
   * crash. This behavior will be changed in the future.
   * @param index2 See index1.
   */
  void playlistMove(int index1, int index2);

  /**
   * @brief Shuffle the current playlist.
   *
   */
  void playlistShuffle();

  /**
   * @brief Clear the current playlist.
   *
   */
  void playlistClear();

  /**
   * @brief Try to pring the current time progressed through a track.
   *
   */
  void printCurrentTime();

  /**
   * @brief Set the currentCtx to be paused.
   *
   */
  void setPaused();

  /**
   * @brief Set the currentCtx to be playing.
   *
   */
  void setPlaying();

  /**
   * @brief Toggle the playing state of the currentCtx.
   *
   */
  void togglePause();
};

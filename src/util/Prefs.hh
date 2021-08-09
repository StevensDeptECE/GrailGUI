#pragma once

#include <string>

class Prefs {
 private:
  std::string baseDir;                       // location of all grail files
  uint32_t preferredWidth, preferredHeight;  // desired window size
  uint32_t preferredX, preferredY;           // desired window location
  bool allowMaximize;           // allow a web page to maximize the window?
  std::string logPath;          // path of the log file
  uint32_t logLevel;            // level of verbosity
  bool enableVoiceCmd;          // will allow Sphinx input
  uint32_t shaderBinaryFormat;  // found binary format for shaders
  bool fastLoadShaders;         // if true, then fast load instead of compiling
  std::string getPath() const;  // get the path for the preferences file
  std::string getGrailDir() const;  // get the path for the preferences file
 public:
  Prefs();
  void load();
  void save();

  bool trySavingShader;  // set true first, if it fails, set false

  std::string getBaseDir() const { return baseDir; }
  std::string getConfDir() const { return baseDir + "conf/"; }
  std::string getFontDir() const { return baseDir + "conf/fonts/"; }
  std::string getShaderDir() const { return baseDir + "shaders/"; }
  uint32_t getPreferredX() const { return preferredX; }
  uint32_t getPreferredY() const { return preferredY; }
  uint32_t getPreferredWidth() const { return preferredWidth; }
  uint32_t getPreferredHeight() const { return preferredHeight; }
  bool getFastLoadShaders() const { return shaderBinaryFormat != 0; }
  void setFastLoadShaders(uint32_t fmt) { shaderBinaryFormat = fmt; }
};

extern Prefs prefs;

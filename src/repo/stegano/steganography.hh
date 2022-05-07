// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>
// bzip2 or lzma
#include <fstream>
#include <iostream>
#include <vector>

// https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp/
// https://github.com/ReneNyffenegger/cpp-base64
#include "base64.h"
#include "opengl/Errcode.hh"
#include "util/Ex.hh"

// std::basic_string<uint8_t>

class SteganographicImage {
 private:
  std::string img_name;

  size_t start = 0;
  size_t stride = 1;

  int w, h;
  size_t img_size;
  uint8_t *rgb;

 public:
  SteganographicImage(const std::string &img_name, size_t start, size_t stride);
  ~SteganographicImage();

  void read_webp();
  void print(const std::vector<uint8_t> &bytes);
  void write_webp(const std::string &output_path);
  void hide_secret(std::vector<uint8_t> &secret);
  std::vector<uint8_t> recover();
};

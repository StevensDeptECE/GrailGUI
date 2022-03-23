// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <fstream>

// std::basic_string<uint8_t>
#include "bytevector.hh"

class SteganographicImage {
 private:
  std::string img_name;

  int start = 0;
  int stride = 1;

  int w, h;
  size_t img_size;
  uint8_t *rgb;

  size_t secret_size;

 public:
  SteganographicImage(const std::string &img_name, int start, int stride);
  ~SteganographicImage();

  void read_webp();
  void write_webp(std::string output_path);

  void hide_secret(byte_vec secret);
  byte_vec recover();
};

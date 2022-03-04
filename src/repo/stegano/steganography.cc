// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <cstring>
#include <fstream>
#include <iostream>

class SteganographicImage {
 private:
  std::string filename;

  int start = 0;
  int stride = 1;

  int w, h;
  size_t filesize;
  uint8_t *rgb, *out;

 public:
  SteganographicImage(const std::string &filename, int start, int stride)
      : filename(filename), start(start), stride(stride) {
    read(filename);
  }

  ~SteganographicImage() { WebPFree(rgb); }

  void read(std::string filename) {
    std::ifstream f(filename, std::ios::binary | std::ios::in);
    if (!f) throw "Input file '" + filename + "' does not exist.";

    // Get byte size of file and return for reading.
    f.seekg(0, std::ios::end);
    filesize = f.tellg();
    f.clear();
    f.seekg(0);

    uint8_t *img = new uint8_t[filesize];
    f.read((char *)img, filesize);

    if (!WebPGetInfo(img, filesize, &w, &h))
      throw "Input image is not a valid WebP file.";
    rgb = WebPDecodeRGB(img, filesize, &w, &h);
    delete[] img;
  }

  // TODO: hide a char *, len - arbitary
  // add len as param - don't wait for null byte
  void hide(char *bytes, size_t len) {
    if (start + len * stride > filesize)
      throw "Input string is too long or stride and start are too large to fit in the image.";
    char bit = 0, c = *bytes++;
    for (int i = start; i < len; i += stride) {
      rgb[i] = c >> (7 - bit) & 1 ? rgb[i] | 1 : rgb[i] & ~1;
      if (++bit == 8) bit = 0, c = *bytes++;
    }
  }

  void write() {
    // NOTE: Doesn't work with transparent webps.
    filesize = WebPEncodeLosslessRGB(rgb, w, h, w * 3, &out);

    // TODO: Add toggle for overwriting file or creating a new one?
    // std::ofstream f("new_" + filename, std::ios::binary | std::ios::out);
    std::ofstream f(filename, std::ios::binary | std::ios::out);
    f.write((char *)out, filesize);
    WebPFree(out);
  }

  char *recover() {
    char bit = 0, c = 0, *bytes = new char[filesize], *j = bytes;
    for (int i = start; i < filesize; i += stride) {
      if (rgb[i] & 1) c |= 1;
      if (++bit == 8)
        bit = 0, *j++ = c, c = 0;
      else
        c <<= 1;
    }
    return bytes;
  }
};

int main(int argc, char **argv) {
  if (argc < 3 || (argc == 4 && argv[1][0] != 'h') ||
      (argc != 4 && argv[1][0] == 'h') || (argc != 3 && argv[1][0] == 'r')) {
    std::cerr << "Usage: " << argv[0]
              << " [h|r] <input.webp> 'string to hide'\n    "
                 "h: Hide string in given image.\n    "
                 "r: Recover string from given image."
              << std::endl;
    return 1;
  }

  // Read in data from file.
  // char *data = (char *)"Hey how are you";
  // char *data = (char *)to_string(123);
  // std::cout << data << std::endl;
  // size_t len = strlen(data);
  try {
    // TODO:
    // - Use a seed to one-time randomize info start and offset.
    // - Deterministically decide start/stride params based on side of image.
    // - Maybe combine both of these to create a sort of random tolerance.
    SteganographicImage steg("aurora_borealis.webp", 200, 5000);
    // steg.hide(data, len);
    steg.write();
    char *rec = steg.recover();
    std::cout << "Recovered message: " << rec << std::endl;
    delete[] rec;
  } catch (char const *e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
  }
  return 0;
}
// Work on upload: Check if downloaded file is diff from original - checksum

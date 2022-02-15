// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <fstream>
#include <iostream>

class SteganographicImage {
 private:
  std::string filename;

  int start = 0;
  // FIXME: Hiding and recovering doesn't work with stride and terminates before
  // end of string.
  int stride = 1;

  int w, h;
  size_t s;
  uint8_t *data;

 public:
  SteganographicImage(std::string filename, int start, int stride)
      : filename(filename), start(start), stride(stride) {
    std::ifstream f(filename, std::ios::binary | std::ios::in);
    if (!f) throw "Input file " + filename + " does not exist.";

    // Get byte size of file and return for reading.
    f.seekg(0, std::ios::end);
    s = f.tellg();
    f.clear();
    f.seekg(0);

    data = new uint8_t[s];
    f.read((char *)data, s);

    if (!WebPGetInfo(data, s, &w, &h))
      throw "Input image is not a valid WebP file.";
    data = WebPDecodeRGB(data, s, &w, &h);
  }

  ~SteganographicImage() { WebPFree(data); }

  void hide(char *str) {
    char bit = 0, c = *str++;
    for (int i = start; i < s && c; i += stride) {
      data[i] = (c >> (7 - bit)) & 1 ? data[i] | 1 : data[i] & ~1;
      if (++bit == 8) bit = 0, c = *str++;
    }

    // NOTE: Doesn't work with transparent webps.
    s = WebPEncodeLosslessRGB(data, w, h, w * 3, &data);

    std::ofstream f("new_" + filename, std::ios::binary | std::ios::out);
    f.write((char *)data, s);
  }

  std::string recover() {
    char bit = 0, c = 0;
    std::string str;
    for (int i = start; i < s; i += stride) {
      if (data[i] & 1) c |= 1;
      if (++bit == 8) {
        // std::cout << c << ": " << (int)c << std::endl;
        if (!c) break;
        bit = 0;
        str += c;
      } else
        c <<= 1;
    }
    return str;
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

  try {
    SteganographicImage steg(argv[2], 0, 1);
    switch (argv[1][0]) {
      case 'h':
        steg.hide(argv[3]);
        break;
      case 'r':
        std::cout << "Recovered message: " << steg.recover() << std::endl;
        break;
      default:
        std::cerr << "Error: Invalid command: " << argv[1] << std::endl;
        std::cerr << "Usage: " << argv[0] << " [h|r] <input.webp>" << std::endl;
        return 1;
    }
  } catch (char const *e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
  }

  return 0;
}

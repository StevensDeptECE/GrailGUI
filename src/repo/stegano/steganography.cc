// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <fstream>
#include <iostream>

class SteganographicImage {
 private:
  std::string filename;

  int start = 100;
  // FIXME: Hiding and recovering doesn't work with stride and terminates before
  // end of string.
  int stride = 1;

  int w, h;
  size_t s;
  uint8_t *data;
  uint8_t *out;

 public:
  SteganographicImage(std::string filename) : filename(filename) {
    std::ifstream fi(filename, std::ios::binary | std::ios::in);
    if (!fi) throw "Input file " + filename + " does not exist.";

    // Get byte size of file and return for reading.
    fi.seekg(0, std::ios::end);
    s = fi.tellg();
    // std::cout << "Number of bytes: " << s << std::endl;
    fi.clear();
    fi.seekg(0);

    data = new uint8_t[s];
    fi.read((char *)data, s);

    if (!WebPGetInfo(data, s, &w, &h))
      throw "Input image is not a valid WebP file.";
    data = WebPDecodeRGB(data, s, &w, &h);
  }

  ~SteganographicImage() {
    WebPFree(data);
    WebPFree(out);
  }

  void hide(char *str) {
    int bit = 0;
    char c = *str++;
    for (int i = start; i < s && c; i += stride) {
      data[i] = (c >> (7 - bit)) & 1 ? data[i] | 1 : data[i] & ~1;
      if (++bit == 8) bit = 0, c = *str++;
    }

    // NOTE: Doesn't work with transparent webps.
    s = WebPEncodeLosslessRGB(data, w, h, w * 3, &out);

    std::ofstream fo("new_" + filename, std::ios::binary);
    fo.write((char *)out, s);
  }

  std::string recover() {
    int bit = 0;
    std::string str;
    char c = 0;
    for (int i = start; i < s; i += stride) {
      if (data[i] & 1) c |= 1;
      if (++bit == 8) {
        std::cout << c << ": " << (int)c << std::endl;
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
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input.webp>" << std::endl;
    return 1;
  }

  char *str = (char *)"Hello, are you there?!";

  try {
    SteganographicImage steg(argv[1]);
    steg.hide(str);

    std::cout << "Recovered message: " << steg.recover() << std::endl;
  } catch (std::string e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
  }

  return 0;
}

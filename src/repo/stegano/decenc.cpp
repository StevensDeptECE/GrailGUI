// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <fstream>
#include <iostream>

int start = 0;
int stride = 1;

uint8_t *hide(uint8_t *data, size_t s, char *str) {
  int bit = 0;
  char c = *str++;
  for (int i = start; i < s && c; i += stride) {
    data[i] = (c >> (7 - bit)) & 1 ? data[i] | 1 : data[i] & ~1;
    if (++bit == 8) bit = 0, c = *str++;
  }
  return data;
}

std::string recover(uint8_t *data, size_t s) {
  int bit = 0;
  std::string str;
  char c = 0;
  for (int i = start; i < s; i += stride) {
    if (data[i] & 1) c |= 1;
    if (++bit == 8) {
      if (!c) break;
      bit = 0;
      str += c;
    } else
      c <<= 1;
  }
  return str;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input.webp>" << std::endl;
    return 1;
  }

  std::ifstream fi(argv[1], std::ios::binary | std::ios::in);

  // Get byte size of file and return for reading.
  fi.seekg(0, std::ios::end);
  size_t s = fi.tellg();
  // std::cout << "Number of bytes: " << s << std::endl;
  fi.clear();
  fi.seekg(0);

  uint8_t *data = new uint8_t[s];
  fi.read((char *)data, s);

  int w, h;
  if (!WebPGetInfo(data, s, &w, &h)) {
    std::cerr << "Error: Input image is not a valid WebP file." << std::endl;
    return 1;
  }
  // std::cout << "width: " << w << std::endl;
  // std::cout << "height: " << h << std::endl;

  data = WebPDecodeRGB(data, s, &w, &h);

  char *str = (char *)"Hello, are you there?!";
  data = hide(data, s, str);

  uint8_t *out;
  // NOTE: these functions, like the lossy versions, use the library's default
  // settings. For lossless this means 'exact' is disabled. RGB values in
  // transparent areas will be modified to improve compression. To avoid this,
  // use WebPEncode() and set WebPConfig::exact to 1.
  s = WebPEncodeLosslessRGB(data, w, h, w * 3, &out);

  std::ofstream fo("new_" + std::string(argv[1]), std::ios::binary);
  fo.write((char *)out, s);

  std::cout << "Recovered message: " << recover(data, s) << std::endl;

  WebPFree(data);
  WebPFree(out);
  return 0;
}

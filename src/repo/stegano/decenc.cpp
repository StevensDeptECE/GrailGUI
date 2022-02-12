// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <fstream>
#include <iostream>

int WEBP_HEADER_SIZE = 16;

uint8_t *hide(uint8_t *data, size_t s, char *str) {
  int bit = 0;
  char c = *str++;
  for (int i = WEBP_HEADER_SIZE; i < s && str; ++i) {
    std::cout << (int)c << std::endl;
    c >>= 7 - bit;
    data[i] = c & 1 ? data[i] | 1 : data[i] & ~1;
    ++bit;
    if (bit == 8) {
      bit = 0;
      c = *str++;
      if (!c) break;
    }
  }
  return data;
}

char *recover(uint8_t *data, size_t s) {
  int bit = 0;
  char *str = new char[4096];
  int n = 0;
  char c = 0;
  for (int i = WEBP_HEADER_SIZE; i < s; ++i) {
    if (data[i] & 1) c |= 1;
    ++bit;
    if (bit == 8) {
      if (c == '\0') break;
      bit = 0;
      str[n++] = c;
    } else
      c <<= 1;
  }
  str[n] = '\0';
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
  // for (int i = 0; i < 36; ++i) std::cout << (char)data[i] << " ";
  std::cout << std::endl;

  int w, h;
  if (!WebPGetInfo(data, s, &w, &h)) {
    std::cerr << "Error: Input image is not a valid WebP file." << std::endl;
    return 1;
  }
  // std::cout << "width: " << w << std::endl;
  // std::cout << "height: " << h << std::endl;

  data = WebPDecodeRGB(data, s, &w, &h);

  char *str = (char *)"hello world";
  data = hide(data, s, str);

  uint8_t *out;
  // NOTE: these functions, like the lossy versions, use the library's default
  // settings. For lossless this means 'exact' is disabled. RGB values in
  // transparent areas will be modified to improve compression. To avoid this,
  // use WebPEncode() and set WebPConfig::exact to 1.
  s = WebPEncodeLosslessRGB(data, w, h, w * 3, &out);

  std::ofstream fo("new_" + std::string(argv[1]), std::ios::binary);
  fo.write((char *)out, s);

  char *recov = recover(data, s);
  printf("%s\n", recov);
  delete[] recov;

  WebPFree(data);
  WebPFree(out);
  return 0;
}

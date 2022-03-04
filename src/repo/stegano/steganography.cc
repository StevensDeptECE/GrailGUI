// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

#include <cstring>
#include <fstream>
#include <iostream>

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
  SteganographicImage(const std::string &img_name, int start, int stride)
      : img_name(img_name), start(start), stride(stride) {
    read_webp();
  }

  ~SteganographicImage() { WebPFree(rgb); }

  void read_webp() {
    std::ifstream f(img_name, std::ios::binary | std::ios::in);
    if (!f) throw "Input file '" + img_name + "' does not exist.";

    // Get byte size of file and return for reading.
    f.seekg(0, std::ios::end);
    img_size = f.tellg();
    f.clear();
    f.seekg(0);

    uint8_t *img = new uint8_t[img_size];
    f.read((char *)img, img_size);

    if (!WebPGetInfo(img, img_size, &w, &h))
      throw "Input image is not a valid WebP file.";
    rgb = WebPDecodeRGB(img, img_size, &w, &h);
    delete[] img;
  }

  // Read in data from file into character array.
  void hide_secret(std::string secret_path) {
    // Instantiate filestream and set pointer to end of file
    std::ifstream f(secret_path, std::ios::binary | std::ios::ate);
    if (!f) throw "Input file '" + secret_path + "' does not exist.";

    f.seekg(0, std::ios::end);
    secret_size = f.tellg();
    // Seek to 0 and read in secret data to an array.
    f.clear();
    f.seekg(0);

    uint8_t *secret = new uint8_t[secret_size];
    f.read((char *)secret, secret_size);

    size_t lim = start + secret_size * stride;
    if (lim > img_size)
      throw "Input string is too long or stride and start are too large to fit in the image.";

    uint8_t bit = 0, c = *secret++;
    for (int i = start; i < lim; i += stride) {
      rgb[i] = c >> (7 - bit) & 1 ? rgb[i] | 1 : rgb[i] & ~1;
      if (++bit == 8) bit = 0, c = *secret++;
    }
  }

  void write_webp(std::string output_path) {
    uint8_t *out;
    // NOTE: Doesn't work with transparent webps.
    img_size = WebPEncodeLosslessRGB(rgb, w, h, w * 3, &out);

    std::ofstream f(output_path, std::ios::binary | std::ios::out);
    f.write((char *)out, img_size);
    WebPFree(out);
  }

  uint8_t *recover() {
    uint8_t bit = 0, c = 0, *secret = new uint8_t[img_size], *j = secret;
    for (int i = start; i < start + secret_size * stride; i += stride) {
      if (rgb[i] & 1) c |= 1;
      if (++bit == 8)
        bit = 0, *j++ = c, c = 0;
      else
        c <<= 1;
    }
    return secret;
  }
};

int main(int argc, char **argv) {
  // if (argc < 3 || (argc == 4 && argv[1][0] != 'h') ||
  //     (argc != 4 && argv[1][0] == 'h') || (argc != 3 && argv[1][0] == 'r')) {
  //   std::cerr << "Usage: " << argv[0]
  //             << " [h|r] <input.webp> 'string to hide'\n    "
  //                "h: Hide string in given image.\n    "
  //                "r: Recover string from given image."
  //             << std::endl;
  //   return 1;
  // }

  try {
    // TODO:
    // - Use a seed to one-time randomize info start and offset.
    // - Deterministically decide start/stride params based on size of image.
    // - Maybe combine both of these to create a sort of random tolerance.
    SteganographicImage steg("aurora_borealis.webp", 0, 1);

    steg.hide_secret("secret.txt");
    steg.write_webp("aurora_borealis.webp");

    uint8_t *rec = steg.recover();
    std::cout << "Recovered message: " << rec << std::endl;
    delete[] rec;

  } catch (char const *e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
  }
  return 0;
}

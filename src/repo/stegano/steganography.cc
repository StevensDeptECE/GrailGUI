// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

// TODO: Need to store the size of the secret at the beginning of the image.
// bzip2 or lzma
#include <fstream>
#include <vector>

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

  void write_webp(std::string output_path) {
    uint8_t *out;
    // NOTE: Doesn't work with transparent webps.
    img_size = WebPEncodeLosslessRGB(rgb, w, h, w * 3, &out);

    std::ofstream f(output_path, std::ios::binary | std::ios::out);
    f.write((char *)out, img_size);
    WebPFree(out);
  }

  // Read in data from file into character array.
  void hide_secret(std::vector<uint8_t> secret) {
    secret_size = secret.size();
    size_t lim = start + secret_size * 8 * stride;
    if (lim > img_size)
      throw "Input string is too long or stride and start are too large to fit in the image.";

    std::vector<uint8_t>::iterator it = secret.begin();
    uint8_t bit = 0, c = *it++;
    for (int i = start; i < lim; i += stride) {
      rgb[i] = c >> (7 - bit) & 1 ? rgb[i] | 1 : rgb[i] & ~1;
      if (++bit == 8) bit = 0, c = *it++;
    }
  }

  std::vector<uint8_t> recover() {
    std::vector<uint8_t> secret;
    uint8_t bit = 0, c = 0;
    for (int i = start; i < start + secret_size * 8 * stride; i += stride) {
      if (rgb[i] & 1) c |= 1;
      ++bit == 8 ? bit = 0, secret.push_back(c), c = 0 : c <<= 1;
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
  std::string img_name = "hubble.webp";
  std::string data_name = "bible.epub";

  try {
    // TODO:
    // - Use a seed to one-time randomize info start and offset.
    // - Deterministically decide start/stride params based on size of image.
    // - Maybe combine both of these to create a sort of random tolerance.
    SteganographicImage steg(img_name, 0, 1);

    std::ifstream in(data_name);
    // std::vector<uint8_t> secret(std::istreambuf_iterator<char>(std::cin),
    // {});
    std::vector<uint8_t> secret(std::istreambuf_iterator<char>(in), {});

    std::cout << "Input size: " << secret.size() << std::endl;

    steg.hide_secret(secret);
    steg.write_webp("new" + img_name);

    std::vector<uint8_t> recov = steg.recover();

    std::ofstream out("new" + data_name);
    for (uint8_t c : recov) out << c;

    std::cout << std::endl;

  } catch (char const *e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
  }
  return 0;
}

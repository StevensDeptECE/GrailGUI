// https://developers.google.com/speed/webp/docs/api
#include <webp/decode.h>
#include <webp/encode.h>

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
    unsigned int secret_size = secret.size();

    int lim = start + secret_size * 8 * stride;
    if (lim > img_size)
      throw "Input string is too long or stride and start are too large to fit in the image.";

    int i;
    uint8_t bit = 0;
    std::vector<uint8_t> secret_size_bytes;
    for (i = 0; i < sizeof(int); i++)
      secret_size_bytes.push_back((secret_size >> (i * 8)) & 0xFF);
    std::vector<uint8_t>::iterator it = secret_size_bytes.begin();
    uint8_t c = *it++;

    // Encode each bit of the secret size
    for (i = start; i < start + stride * sizeof(int) * 8; i += stride) {
      rgb[i] = c >> (7 - bit) & 1 ? rgb[i] | 1 : rgb[i] & ~1;
      if (++bit == 8) bit = 0, c = *it++;
    }

    c = *(it = secret.begin())++, bit = 0;

    for (i += stride; i < lim; i += stride) {
      rgb[i] = c >> (7 - bit) & 1 ? rgb[i] | 1 : rgb[i] & ~1;
      if (++bit == 8) bit = 0, c = *it++;
    }
  }

  std::vector<uint8_t> recover() {
    int i;

    std::vector<uint8_t> secret_size_bytes;
    uint8_t bit = 0, c = 0;
    for (i = start; i < start + stride * sizeof(int) * 8; i += stride) {
      if (rgb[i] & 1) c |= 1;
      ++bit == 8 ? bit = 0, secret_size_bytes.push_back(c), c = 0 : c <<= 1;
    }

    unsigned int secret_size = 0;
    for (i = 0; i < sizeof(int); i++)
      secret_size |= secret_size_bytes[i] << (i * 8);

    std::vector<uint8_t> secret;
    for (i += stride; i < start + secret_size * 8 * stride; i += stride) {
      if (rgb[i] & 1) c |= 1;
      ++bit == 8 ? bit = 0, secret.push_back(c), c = 0 : c <<= 1;
    }
    return secret;
  }
};

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <input.webp> <file to hide>"
              << std::endl;
    return 1;
  }
  std::string img_name = argv[1];
  std::string data_name = argv[2];

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

  } catch (char const *e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
  }
  return 0;
}

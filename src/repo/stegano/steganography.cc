#include "steganography.hh"

SteganographicImage::SteganographicImage(const std::string &img_name,
                                         size_t start, size_t stride)
    : img_name(img_name), start(start), stride(stride) {
  read_webp();
}

SteganographicImage::~SteganographicImage() { WebPFree(rgb); }

void SteganographicImage::read_webp() {
  std::ifstream f(img_name, std::ios::binary | std::ios::in);
  if (!f) throw Ex2(Errcode::FILE_NOT_FOUND, img_name);

  // Get byte size of file and return for reading.
  f.seekg(0, std::ios::end);
  img_size = f.tellg();
  f.clear();
  f.seekg(0);

  uint8_t *img = new uint8_t[img_size];
  f.read((char *)img, img_size);

  if (!WebPGetInfo(img, img_size, &w, &h))
    // Input image is not a valid WebP file.
    throw Ex2(Errcode::FILE_READ, img_name);
  rgb = WebPDecodeRGB(img, img_size, &w, &h);
  delete[] img;
}

void SteganographicImage::print(const std::vector<uint8_t> &bytes) {
  std::cout << base64_encode(bytes.data(), bytes.size()) << std::endl;
}

void SteganographicImage::write_webp(const std::string &output_path) {
  uint8_t *out;
  // NOTE: Doesn't work with transparent webps.
  img_size = WebPEncodeLosslessRGB(rgb, w, h, w * 3, &out);

  std::ofstream f(output_path, std::ios::binary | std::ios::out);
  f.write((char *)out, img_size);
  WebPFree(out);
}

// Read in data from file into character array.
// TODO: Some future ideas:
//    - Allow ability to split secret over multiple images.
void SteganographicImage::hide_secret(std::vector<uint8_t> &secret) {
  size_t secret_size = secret.size();

  size_t lim = start + secret_size * 8 * stride + sizeof(size_t) * 8;
  if (lim > img_size)
    // Input string is too long or stride/start are too big to fit in img.
    throw Ex1(Errcode::STRING_TOO_LONG);

  size_t i;
  uint8_t bit = 0;
  std::vector<uint8_t> secret_size_bytes;
  for (i = 0; i < sizeof(size_t); ++i)
    secret_size_bytes.push_back((secret_size >> (i * 8)) & 0xFF);
  std::vector<uint8_t>::iterator it = secret_size_bytes.begin();
  uint8_t c = *it++;

  // Encode each bit of the secret size
  for (i = start; i < start + stride * sizeof(size_t) * 8; i += stride) {
    rgb[i] = c >> (7 - bit) & 1 ? rgb[i] | 1 : rgb[i] & ~1;
    if (++bit == 8) bit = 0, c = *it++;
  }

  c = *(it = secret.begin())++, bit = 0;

  for (i += stride; i < lim + 8; i += stride) {
    rgb[i] = c >> (7 - bit) & 1 ? rgb[i] | 1 : rgb[i] & ~1;
    if (++bit == 8) bit = 0, c = *it++;
  }
}

std::vector<uint8_t> SteganographicImage::recover() {
  size_t i;

  // Grab the bytes from the image that store the secret's size.
  std::vector<uint8_t> secret_size_bytes;
  uint8_t bit = 0, c = 0;
  for (i = start; i < start + stride * sizeof(size_t) * 8; i += stride) {
    if (rgb[i] & 1) c |= 1;
    ++bit == 8 ? bit = 0, secret_size_bytes.push_back(c), c = 0 : c <<= 1;
  }

  // Convert the secret's size bytes to an integer.
  size_t secret_size = 0;
  for (int j = 0; j < sizeof(size_t); ++j)
    secret_size |= secret_size_bytes[j] << (j * 8);

  std::vector<uint8_t> secret;
  for (i += stride;
       i < start + sizeof(size_t) * 8 + secret_size * 8 * stride + 8;
       i += stride) {
    if (rgb[i] & 1) c |= 1;
    ++bit == 8 ? bit = 0, secret.push_back(c), c = 0 : c <<= 1;
  }
  return secret;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <input.webp> <file to hide>"
              << std::endl;
    return 1;
  }
  std::string img_name = argv[1], data_name = argv[2];

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

    steg.hide_secret(secret);
    steg.write_webp("new" + img_name);

    std::vector<uint8_t> recov = steg.recover();

    std::ofstream out("new" + data_name);
    for (uint8_t c : recov) out << c;

    steg.print(recov);

  } catch (const std::string &e) {
    std::cerr << "Error: " << e << std::endl;
    return 1;
  }

  return 0;
}

#include "repo/stegano/Steganography.hh"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <input.webp> <file to hide>"
              << std::endl;
    return 1;
  }
  std::string img_name = argv[1], data_name = argv[2];

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

  return EXIT_SUCCESS;
}

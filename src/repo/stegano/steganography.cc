#include <cstring>
#include <iostream>

// https://imagemagick.org/Magick++/tutorial/Magick++_tutorial.pdf
#include <Magick++.h>

class SteganographicImage {
 private:
  int w, h, c;
  Magick::Image img;
  Magick::Quantum *pix;
  std::string filename;
  uint32_t start = 0;   // start of the data (x=10,y=13) 13*w+h
  uint32_t stride = 1;  // how many to skip
  // uint32_t bits_per_color = 1;
  uint64_t *data;
  uint32_t len;

 public:
  SteganographicImage(std::string filename, uint64_t *data, uint32_t len)
      : filename(filename), data(data), len(len) {
    Magick::InitializeMagick("");
    img.read(filename);
    h = img.rows(), w = img.columns();
    c = img.channels();
    pix = img.getPixels(0, 0, w, h);
  }

  ~SteganographicImage() {}

  void hide() {
    for (uint32_t i = 0; i < len; ++i) {
      uint32_t pos = start + i * stride;
      /* std::cout << pix[pos] << std::endl; */
      pix[pos] =
          (uint32_t)pix[pos] & 0b111111101111111011111110;  // Remove LSBs
      pix[pos] = (uint32_t)pix[pos] |
                 (data[i] & 1);  // Encode data into picture 1 bit at a time.
      // pix[pos] = (uint32_t) pix[pos] | (((v >> 1) & 1) << 8); // or in the
      // low green bit pix[pos] = (uint32_t) pix[pos] | (((v >> 2) & 1) << 16);
    }
    img.syncPixels();
  }

  uint32_t *recover() {
    uint32_t *recovered = new uint32_t[len];
    for (uint32_t i = 0; i < len; ++i)
      recovered[i] = (uint32_t)pix[start + i * stride] & 1;
    return recovered;
  }

  void write() {
    std::string new_filename =
        filename.substr(0, filename.find_last_of(".")) + ".png";
    img.write(new_filename);
    std::cout << "File written to " << new_filename << std::endl;
  }
};

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }
  std::string filename = argv[1];
  // Data to hide in message and length of message.
  char *msg = (char *)"hellohellohellohellohello";
  int n = strlen(msg);

  uint64_t *data = new uint64_t[n];
  for (int i = 0; i < n; ++i) data[i] = (int)msg[i];

  try {
    SteganographicImage steg = SteganographicImage(filename, data, n);
    steg.hide();
    // Write a new image storing the secret message.
    steg.write();
    // Recover the secret message from the picture and print it out.
    auto recovered = steg.recover();
    for (int i = 0; i < n; ++i) std::cout << (char)recovered[i] << " ";
    std::cout << std::endl;
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}

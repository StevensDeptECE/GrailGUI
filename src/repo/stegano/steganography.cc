#include <iostream>
#include <cstring>

// https://imagemagick.org/Magick++/tutorial/Magick++_tutorial.pdf
#include <Magick++.h>

class SteganographicImage {
private:
    int w, h, c;
    Magick::Image img;
    Magick::PixelPacket *pix;
    char *filename;
    uint32_t start = 0; // start of the data (x=10,y=13) 13*w+h
    uint32_t stride = 5; // how many to skip
    // uint32_t bits_per_color = 1;
    uint64_t *data;
    uint32_t len;

public:
    SteganographicImage(char *filename, uint64_t *data, uint32_t len) :
        filename(filename), data(data), len(len) {
            img.read(filename);
            h = img.rows(),
            w = img.columns();
            c = img.channels();
            pix = img.getPixels(0, 0, h, w);
        }

    ~SteganographicImage() { }

    void hide() {
        for (uint32_t i = 0; i < h; ++i)
            for (uint32_t j = 0; j < w; ++j)
                for (uint32_t k = 0; k < len; ++k) {
                    uint32_t pos = start + k * stride;
                    uint64_t v = data[k]; // v is what you put into your image
                    pix[pos] &= 0b111111101111111011111110; // wipe out the low bit of each r,g,b
                    pix[pos] |= v & 1; // get each rightmost bit and put 1 bit into low part of blue
                    pix[pos] |= ((v >> 1) & 1) << 8; // or in the low green bit
                    pix[pos] |= ((v >> 2) & 1) << 16;
                }
        img.syncPixels();
    }

    uint32_t* recover() {
        uint32_t *recovered = new uint32_t[len];
        for (uint32_t i = 0; i < len; ++i) {
            uint32_t pos = start + i * stride;
            recovered[i] = pix[i] & 1;
        }
        return recovered;
    }

    void write() {
        img.write(filename);
        std::cout << "File written to " << filename << std::endl;
    }
};

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    char *filename = argv[1];
    // Data to hide in message and length of message.
    char *msg = (char *)"hello";
    int n = strlen(msg);
    uint64_t *data = new uint64_t[n];
    for (int i = 0; i < n; ++i)
        data[i] = (int) msg[i];

    try {
        SteganographicImage steg = SteganographicImage(filename, data, n);
        steg.hide();
        // Write a new image storing the secret message.
        steg.write();
        // Recover the secret message from the picture and print it out.
        auto recovered = steg.recover();
        for (int i = 0; i < n; ++i)
            std::cout << (char) recovered[i] << " ";
        std::cout << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

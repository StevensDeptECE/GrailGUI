#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>

class SteganographicImage {
private:
    int w = 0, h = 0, c = 0;
    char *filename;
    uint32_t start = 0; // start of the data (x=10,y=13) 13*w+h
    uint32_t stride = 2; // how many to skip =2
    // uint32_t bits_per_color = 1;
    uint8_t *data;
    uint32_t len;

public:
    unsigned char *rgb;
    SteganographicImage(char *filename, uint8_t *data, uint32_t len) :
        filename(filename), data(data), len(len) {
            rgb = stbi_load(filename, &w, &h, &c, 0);
        }

    ~SteganographicImage() {
        stbi_image_free(rgb);
    }

    void hide() {
        for (uint32_t i = start; i < len * stride; i += stride) {
            auto id = (start + i) / stride;
            if (!data[id])
                rgb[i] &= data[id] + 0b11111110;
            else
                rgb[i] |= data[id];
        }
    }

    uint32_t* recover() {
        uint32_t *recovered = new uint32_t[len];
        for (int i = start; i < len * stride; i += stride)
            recovered[(start + i) / stride] = rgb[i] & 1;
        return recovered;
    }

    void write() {
        stbi_write_jpg(filename, w, h, c, rgb, w * c);
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
    // Test 1: Binary string.
    uint8_t data[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    int n = 10;

    // Test 2: ASCII text.
    // char *msg = (char *)"hello";
    // int n = strlen(msg);
    // uint8_t *data = new uint8_t[n];
    // for (int i = 0; i < n; ++i)
    //     data[i] = (int) msg[i];

    SteganographicImage steg = SteganographicImage(filename, data, n);
    if (!steg.rgb) {
        std::cerr << "Error: " << filename << " does not exist." << std::endl;
        return 1;
    }
    steg.hide();
    // Write a new image storing the secret message.
    steg.write();
    // Recover the secret message from the picture and print it out.
    auto *recovered = steg.recover();
    for (int i = 0; i < n; ++i)
        std::cout << recovered[i] << " ";
    std::cout << std::endl;
    return 0;
}

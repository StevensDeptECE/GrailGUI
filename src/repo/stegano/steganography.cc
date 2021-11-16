#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <iostream>

class SteganographicImage {
private:
    int w, h, c = 3;
    unsigned char *rgb;
    char *filename;
    uint32_t start = 0; // start of the data (x=10,y=13) 13*w+h
    uint32_t stride = 2; // how many to skip =2
    uint32_t bits_per_red; // 2
    uint32_t bits_per_green; // 2
    uint32_t bits_per_blue; // 2

public:
    SteganographicImage(char *filename)
        : filename(filename) {
            this->filename = filename;
            rgb = new uint8_t[w * h * 3];
    }

    ~SteganographicImage(){
        stbi_image_free(rgb);
    }

    void hide(const uint8_t data[], uint32_t len) {
        // for (int i = start; i < h; i += stride) {
        //     for (int j = start; j < w; j += stride) {
        //         for (int k = 0; k < 3; ++k) {
        //             auto pixc = rgb[i][j];
        //             rgb[i][j][k] = pixc;
        //         }
        //     }
        // }
        for (int i = 0; i < len; ++i)
            if (!data[i])
                rgb[i] &= data[i] + 0b11111110;
            else
                rgb[i] |= data[i] + 0b00000000;
    }

    void recover(uint8_t data[], uint32_t len) {
        for (int i = 0; i < len; ++i)
            data[i] = rgb[i] & 1;
    }

    void write() {
        stbi_write_jpg(filename, w, h, c, rgb, w * c);
        std::cout << "File written to " << filename << std::endl;
    }

    int read() {
        rgb = stbi_load(filename, &w, &h, &c, 0);
        return (bool)rgb;
    }

};

int main(int argc, char **argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    char *filename = argv[1];
    SteganographicImage test = SteganographicImage(filename);
    if (!test.read()) {
        std::cerr << "Error: " << filename << " does not exist." << std::endl;
        return 1;
    }
    uint8_t d[] = {0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
    test.hide(d, 10);
    test.write();
    return 0;
}

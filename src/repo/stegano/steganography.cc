#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

class SteganographicImage {
private:
    uint32_t w, h;
    uint8_t* rgb;
    uint32_t start; // start of the data (x=10,y=13) 13*w+h
    uint32_t stride; // how many to skip =2
    uint32_t bits_per_red; // 2
    uint32_t bits_per_green; // 2
    uint32_t bits_per_blue; // 2
    unsigned char *data;

    bool isBitSet(char ch, int pos) {
        return ch >>= pos & 1;
    }
public:
    SteganographicImage(uint32_t w, uint32_t h, const char* img)
        : w(w), h(h), rgb(new uint8_t[w * h * 3]) {
            memcpy(rgb, img, w * h * 3);
    }

    ~SteganographicImage(){
        stbi_image_free(data);
    }

    void hide(const uint8_t data[], uint32_t len) {
        char ch;
        
        for (int i = start; i < h; i += stride) {
            for (int j = start; j < w; j += stride) {
                for (int k = 0; k < 3; ++k) {
                    auto pixc = rgb[i][j];
                    rgb[i][j][k] = pixc;
                }
            }
        }
    }

    void recover(uint8_t data[], uint32_t len) {
    }

    void write() {
        stbi_write_jpg("sky2.jpg", w, h, channels, rgb, 100);
    }

    void read(char *filename) {
        int x, y, n;
        data = stbi_load(filename, &x, &y, &n, 0);
        w = x;
        y = h;
    }
};

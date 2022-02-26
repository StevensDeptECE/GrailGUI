#include <webp/decode.h>
#include <webp/encode.h>

#include <fstream>
#include <iostream>
#include <random>

#include "bitmap.hh"

using namespace std;
// int WebPGetInfo(const uint8_t* data, size_t data_size, int* width, int*
// height); size_t WebPEncodeRGB(const uint8_t* rgb, int width, int height, int
// stride, float quality_factor, uint8_t** output);

bitmap::bitmap(uint32_t w, uint32_t h, uint32_t color)
    : w(w), h(h), rgb(new uint32_t[w * h]) {
  for (uint32_t i = 0; i < w * h; i++) rgb[i] = color;
}

bitmap::~bitmap() { delete[] rgb; }

std::default_random_engine gen;

void bitmap::random() {
  // std::uniform_int_distribution<uint32_t>
  // dist(0,numeric_limits<uint32_t>::max());
  for (uint32_t i = 0; i < w * h; i++) {
    rgb[i] = gen();
  }
}

void bitmap::horiz_line(uint32_t x1, uint32_t x2, uint32_t y, uint32_t color) {
  uint32_t start = y * w + x1, end = y * w + x2;
  for (uint32_t p = start; p <= end; p++) rgb[p] = color;
}

void bitmap::save(const char filename[]) {
  uint8_t* out;
  size_t s = WebPEncodeRGB((uint8_t*)rgb, w, h, 0, 100, &out);
  ofstream f(filename, ios::binary);
  f.write((char*)out, s);
  WebPFree(out);
}

void bitmap::load(const char filename[]) {
  uint8_t* out;
  int s = WebPGetInfo(rgb, w * h * 4, w, h, out);
}

int main() {
  bitmap b(1024, 800, 0xFF0000);
  //	b.random();
  b.horiz_line(100, 400, 50, 0xFF000080);
  b.save("test.webp");
}

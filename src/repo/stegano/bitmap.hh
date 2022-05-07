#include <iostream>

class bitmap {
 private:
  int w, h;
  uint32_t* rgb;  // pointer to the data rgba?
 public:
  bitmap(uint32_t w, uint32_t h, uint32_t color);
  bitmap(const char filename[]);
  ~bitmap();
  bitmap(const bitmap& orig) = delete;
  bitmap& operator=(const bitmap& orig) = delete;

  void load(const char filename[]);
  void save(const char filename[]);

  // fill the image with random bits
  void random();

  // set the entire bitmap to a color
  void clear(uint32_t color);
  void fill_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h,
                 uint32_t color);
  void horiz_line(uint32_t x1, uint32_t x2, uint32_t y, uint32_t color);
  void vert_line(uint32_t y1, uint32_t y2, uint32_t x, uint32_t color);
  void line(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t color);
  void rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);

  // draw a rounded rectangle with corner radius r
  void round_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t r,
                  uint32_t color);
};

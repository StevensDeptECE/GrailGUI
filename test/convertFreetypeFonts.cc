#include "opengl/GLWinFonts.hh"
#include "grail/Errors.hh"
#include "util/Ex.hh"
#include <ft2build.h>
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"


void fontCleanup() {
  FT_Done_FreeType(ftLib);
}

int main() {
  try {
    string paths[] = {"Google"};
    uint32_t sizes[] = {12,14,16,20,24,30, 40};
    FontFace::loadFonts("TIMES",sizes,7);
    fontCleanup();
  } catch (const Ex& e) {
    cout << e << '\n';
  }
}

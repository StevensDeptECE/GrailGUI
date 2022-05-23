#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include "opengl/GLWinFonts.hh"

#include <dirent.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <memory>

#include "glad/glad.h"
#include "opengl/GLWin.hh"
#include "util/Ex.hh"
#include "util/FileUtil.hh"
using namespace std;

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

unordered_map<string, uint32_t> FontFace::faceByName;

// std::vector<std::string> FontFace::loaded_fonts;
std::vector<FontFace*> FontFace::faces;
FT_Library FontFace::ftLib;

Font::Font(FontFace* face, FT_Face ftFace, uint16_t height, uint8_t bitmap[],
           uint32_t& sizeX, uint32_t& sizeY, uint32_t& currX, uint32_t& currY,
           uint32_t& rowSize)
    : parentFace(face), height(height), startGlyph(32) {
#if 0
  numGlyphs = 0;
  const uint32_t startGlyph = 0;
  const uint32_t endGlyph = 200;
  if (face->maxWidthIndex == 0) {
    maxWidth = 0;
    const uint32_t maxWidthIndex = 0;
    for (uint32_t c = startGlyph; c <= endGlyph; c++) {
      if (!FT_Load_Char(ftFace, c, FT_LOAD_RENDER)) {
        if (ftFace->glyph->bitmap.width > maxWidth) {
          maxWidth = ftFace->glyph->bitmap.width;
          face->maxWidthIndex = maxWidthIndex;
        }
        //verticalPixelsUsed += ftFace->glyph->bitmap.rows;
        verticalPixelsUsed += height;
        numGlyphs++;
      }
      face->numGlyphs = numGlyphs;
    }
  } else if (!FT_Load_Char(ftFace, face->maxWidthIndex, FT_LOAD_RENDER)) {
    maxWidth = ftFace->glyph->bitmap.width;
    numGlyphs = face->numGlyphs;
  }
  spaceWidth = maxWidth / 2;  // TODO: Fixed width should be = maxWidth;
  totalHeight = verticalPixelsUsed;
#endif
  /* Hashmap to compare to existing glyphs
     for a glyph that is the same, all subsequent glyphs refer to the
     position of the first one. Typically this only happens for a
     placeholder rectangle for characters not supported by the font
  */
  const uint32_t startGlyph = 0;
  const uint32_t endGlyph = 200;
  unordered_map<uint32_t, uint32_t> glyphMap;
  maxWidth = 0;
  spaceWidth = height / 2;
  for (uint32_t c = startGlyph; c <= endGlyph; c++) {
    addGlyph(ftFace, glyphMap, c, bitmap, sizeX, sizeY, currX, currY, rowSize);
  }
}

Font::~Font() {}

Font* Font::getDefault() {
  return nullptr;  // TODO: set default font
}

// TODO: can be made a private method of font
static uint32_t hashGlyph(const uint8_t glyphBuffer[], uint32_t len) {
  uint32_t hash = len;
  for (int i = 0; i < len; i++)
    hash = ((hash << 17) | (hash >> 13)) ^ ((hash << 5) | (hash >> 27)) ^
           glyphBuffer[i];
  return hash;
}

void Font::addGlyph(FT_Face ftFace, unordered_map<uint32_t, uint32_t>& glyphMap,
                    uint8_t c, uint8_t bitmap[], uint32_t& sizeX,
                    uint32_t& sizeY, uint32_t& currX, uint32_t& currY,
                    uint32_t& rowSize) {
  FT_BitmapGlyph bg;

  // Loads glyph and pushes a blank if anything fails.
  if (FT_Load_Char(ftFace, c, FT_LOAD_RENDER)) {
    char character[2] = {(char)c, '\0'};
    cerr << "Failed to load glyph for c=" << c << '\n';
    // throw Ex2(Errcode::FONT_LOAD_GLYPH, glyph);
    glyphs.push_back(Glyph(maxWidth, 0, 0, 0, 0, 0.0, 0.0, 1.0, 1.0));
    return;
  } else {
    ///* convert to an anti-aliased bitmap */

    // The below code used to work, but is specifically not to be used with the
    // glyph format FT_GLYPH_FORMAT_BITMAP. As a result, use FT_Get_Glyph and
    // FT_Glyph_To_Bitmap instead, but the original code here is left commented
    // out in the event that this new code breaks again

    // if (FT_Render_Glyph(ftFace->glyph, FT_RENDER_MODE_NORMAL)) {
    // std::cerr << "ERROR::FREETYPE: Failed to Render Glyph" << std::endl;
    FT_Glyph glyph;
    if (FT_Get_Glyph(ftFace->glyph, &glyph)) {
      std::cerr << "ERROR::FREETYPE: Failed to get glyph" << std::endl;
      glyphs.push_back(Glyph(maxWidth, 0, 0, 0, 0, 0.0, 0.0, 1.0, 1.0));
      return;
    }
    if (FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 0)) {
      std::cerr << "ERROR::FREETYPE: Failed to convert glyph to bitmap"
                << std::endl;
      glyphs.push_back(Glyph(maxWidth, 0, 0, 0, 0, 0.0, 0.0, 1.0, 1.0));
      return;
    }
    bg = (FT_BitmapGlyph)glyph;
  }

  // The below is from the same code as the above commented out FT_Render_Glyph.
  // Use this if we have to switch back to using FT_Render_Glyph. All of the
  // functions that rely on bg will have to be changed in that instance, but at
  // this moment that means about 6 or 7 changes isolated to Font::addGlyph and
  // nothing anywhere else.

  // FT_GlyphSlot g = ftFace->glyph;
  // const uint8_t* freetype_buffer_bd = g->bitmap.buffer;
  const uint8_t* freetype_buffer_bd = bg->bitmap.buffer;
  if (freetype_buffer_bd == nullptr) {  // render succeeds and it's still null?
    // cerr << "buffer is null for character " << int(c) << '\n';
    // this is for space and non-printing characters
    glyphs.push_back(
        Glyph(spaceWidth, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    // TODO: this never happens either
    return;
  }

  uint32_t h = bg->bitmap.rows;
  uint32_t w = bg->bitmap.width;
  if (w > maxWidth) maxWidth = w;
  uint32_t glyphSize = h * w;
  uint32_t newHash = hashGlyph(freetype_buffer_bd, glyphSize);
  auto matchGlyphIt = glyphMap.find(newHash);
  if (matchGlyphIt == glyphMap.end()) {  // not found, add this one.
    // TODO: There is the small and statistically wildly improbable case where
    // the two hashes match but the glyphs are not the same
    // we are ignoring that for now. It is quite unlikely.
    glyphMap[newHash] = c;  // the hash map goes to this character
    float advance = float(ftFace->glyph->metrics.horiAdvance >> 6);
    // glm::ivec2 bearing = glm::ivec2(g->bitmap_left, g->bitmap_top);
    // glm::ivec2 size = glm::ivec2(w, h);
    if (currX + w > sizeX) {
      currX = 0;
      currY += rowSize;
      rowSize = 0;
    }
    glyphs.push_back(Glyph(
        advance, bg->left, bg->top,  // bearing x and y
        w, h,                        // sizeX and sizeY
        float(double(currX) / sizeX), float(double(currX + w - 1) / sizeX),
        float(double(currY) / sizeY), float(double((currY + h)) / sizeY)
        //            float(double(sizeY - currY) / sizeY),
        //            float(double(sizeY - (currY+h)) / sizeY)
        ));
    uint32_t dest = currY * sizeX + currX;
    for (uint32_t row = 0; row < h; row++, dest += sizeX) {
      memcpy(bitmap + dest, freetype_buffer_bd + row * w, w);
    }
    if (h > rowSize) {  // store the largest character on this row
      rowSize = h;      // when advancing to next row, add rowSize
    }  // TODO: Note this means we waste space. could pack better
    currX += w;
  } else {
    // copy the glyph from the earlier equivalent
    glyphs.push_back(glyphs[matchGlyphIt->second]);
  }
}

float Font::getWidth(const char text[], uint32_t len) const {
  float w = 0;
  for (const char* p = text; len > 0; len--, p++) w += getGlyph(*p)->sizeX;
  return w;
}

FontFace::FontFace(FT_Library ft, const string& faceName,
                   const string& facePath, uint32_t minFontSize, uint32_t inc,
                   uint32_t maxFontSize, uint8_t bitmap[], uint32_t& sizeX,
                   uint32_t& sizeY, uint32_t& currX, uint32_t& currY,
                   uint32_t& rowSize)
    : faceName(faceName), boldness(0), italics(0), fixed(0), maxWidthIndex(0) {
  FT_Face ftFace;
  if (FT_New_Face(ft, facePath.c_str(), 0,
                  &ftFace)) {  // load in the face using freetype
    cerr << "Failed to load font: " << facePath << '\n';
    return;  // TODO: throw
  }
  pathByName[faceName] = facePath;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  for (uint32_t fontSize = minFontSize, i = 0; fontSize <= maxFontSize;
       fontSize += inc, i++) {
    FT_Set_Pixel_Sizes(ftFace, 0,
                       fontSize);  // create a FreeType font of sizes[i]
    addFont(new Font(this, ftFace, fontSize, bitmap, sizeX, sizeY, currX, currY,
                     rowSize));
    fontBySize[fontSize] = i;
  }

  faces.push_back(this);
  uint32_t faceId = faces.size() - 1;
  faceByName[faceName] = faceId;

  FT_Done_Face(ftFace);  // clean up FreeType when done
}

FontFace::~FontFace() {
  for (auto f : fonts) delete f;
}

void FontFace::emptyFaces() {
  for (auto face : faces) delete face;
  faces.clear();
}

unordered_map<string, string> FontFace::pathByName;

void FontFace::setTexture(const uint8_t bitmap[], uint32_t w, uint32_t h) {
  uint32_t sharedTextureId;
  glGenTextures(1,
                &sharedTextureId);  // create a single textureid with all fonts
  glBindTexture(GL_TEXTURE_2D, sharedTextureId);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Disable byte-alignment restriction
  // Set texture options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D,
               0,       // top level (no mip-mapping)
               GL_RED,  // internal format
               w,       // width of the entire bitmap, all fonts side by side
               h,       // vert size of the texture.
               // could be really big, is that a problem?
               0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

  glGenerateMipmap(GL_TEXTURE_2D);

  for (auto face : faces) {
    face->textureId = sharedTextureId;
    for (auto font : face->fonts) {
      font->textureId = sharedTextureId;
    }
  }
  constexpr int CHANNELS = 1;
  stbi_write_png("tmp/combined.png", w, h, CHANNELS, bitmap,
                 w * CHANNELS);  // TODO: Remove - debugging purposes
}

ostream& operator<<(ostream& s, const Font::Glyph& g) {
  return s << "advance: " << g.advance << '\n'
           << "bearing: " << g.bearingX << "," << g.bearingY << '\n'
           << "size:    " << g.sizeX << "," << g.sizeY << '\n'
           << "x:       " << g.u0 << "," << g.u1 << '\n'
           << "top: " << g.v1 << '\n'
           << "bot: " << g.v0 << '\n';
}

#if 0
bool FontFace::combineFaces() {
  uint32_t maxHeightAllFaces = 0;
  uint32_t totalWidthAllFaces = 0;
  for (const auto &face : faces) {
    if (face->maxHeight > maxHeightAllFaces)
      maxHeightAllFaces = face->maxHeight;
    totalWidthAllFaces += face->totalWidth;
  }
  totalWidthAllFaces =
      (totalWidthAllFaces + 7) / 8 *
      8;  // round up to next multiple of 8 so each row is 64-bit aligned
  uint8_t *combinedBitmap = new uint8_t[(maxHeightAllFaces+1) * totalWidthAllFaces];
  memset(combinedBitmap, 0, maxHeightAllFaces * totalWidthAllFaces);
  cout << "dimensions of bigmap.png should be: " << maxHeightAllFaces << " x "
       << totalWidthAllFaces << '\n';
  if (combinedBitmap == nullptr) {
    cerr << "Error allocating memory for fonts\n";
    return false;
  }

  uint64_t destStartCol = 0;
  for (const auto &face : faces) {
    for (auto bd : face->bitmaps) {
      int src = 0;
      int dest = destStartCol;
      for (int row = 0; row < bd.compressedHeight;
           row++, src += bd.width, dest += totalWidthAllFaces) {
        memcpy(combinedBitmap + dest, bd.bitmap + src, bd.width);
      }
      destStartCol += bd.width;
    }
  }

cout << "Combined total size:" << totalWidthAllFaces << "," << maxHeightAllFaces << '\n';
  setCombinedTexture(combinedBitmap, totalWidthAllFaces, maxHeightAllFaces);

  /* edit u0 and u1 of each Glyph to the fractional values */
  float currPos = 0.0;
  float nextPos = 0.0;
  const float invTotalWidth = 1.0 / totalWidthAllFaces;
  int currentLeftPos = 0;  // bit position of left edge of current font
  for (auto face : faces) {
    int i = 0;
    for (Font *font : face->fonts) {
      // accumulate the width
      // TODO: is right edge of this font = currentLeftposition + width-1????
      // how do float texture coords work?
      currPos = float(currentLeftPos) * invTotalWidth;
      nextPos = float(currentLeftPos + font->maxWidth - 1) * invTotalWidth;
      //      nextPos = currPos + float(font->maxWidth) * invTotalWidth;   //cout << "xxxcurr, next: " << currPos << ", " << nextPos << '\n';
      double heightRatio = maxHeightAllFaces / double(face->bitmaps[i].height);
      for (Font::Glyph &glyph : font->glyphs) {
        // change u0 and u1
        glyph.u0 = currPos;
        glyph.u1 = nextPos;  // TODO: see above, this might be slightly off

        // change v0 and v1
        glyph.v0 = glyph.v0*heightRatio;  // f = 8039 / 8040  *  5360/ 8040
        glyph.v1 = glyph.v1*heightRatio;
      }
      // advance to the next strip
      // currPos = nextPos;
      i++;
      currentLeftPos += font->maxWidth;
    }
  }
  saveFonts(combinedBitmap, totalWidthAllFaces, maxHeightAllFaces);
  delete[] combinedBitmap;
  return true;
}
#endif

void Font::save(ostream& fastfont) {
  fastfont.write((char*)&height, sizeof(uint16_t));
  fastfont.write((char*)&startGlyph, sizeof(uint32_t));
  uint32_t numGlyphs = glyphs.size();
  fastfont.write((char*)&numGlyphs, sizeof(uint32_t));
  fastfont.write((char*)&glyphs[0], glyphs.size() * sizeof(Glyph));
}
Font::Font(istream& fastfont) {
  fastfont.read((char*)&height, sizeof(uint16_t));
  fastfont.read((char*)&startGlyph, sizeof(uint32_t));
  uint32_t numGlyphs = glyphs.size();
  fastfont.read((char*)&numGlyphs, sizeof(uint32_t));
  glyphs.reserve(numGlyphs);
  for (int i = 0; i < numGlyphs; i++) {
    Glyph g;
    fastfont.read((char*)&g, sizeof(Glyph));
    glyphs.push_back(g);
#if 0
  glyphs.insert(glyphs.begin(),
               istream_iterator<Glyph>(fastfont),
               istream_iterator<Glyph>());
#endif
  }
}

void FontFace::save(ostream& fastfont) {
  fastfont << faceName << boldness << italics << fixed;
  for (auto font : fonts) {
    font->save(fastfont);
  }
}

/*
  Format for header of the binary file to fastload fonts
*/
struct FastFontHeader {
  uint32_t magic;      // the magic number identifying this file
  uint32_t version;    // version number of the implementation
  uint32_t w, h;       // width and height of the bitmap
  uint32_t numFaces;   // number of individual font faces
  uint32_t numFonts;   // number of fonts within those faces (collectively)
  uint32_t numGlyphs;  // total number of glyphs
};

/*
 save all faces, each font in each face, and the bitmap into a
 single binary file that can be rapidly loaded
*/
void FontFace::saveFonts(const uint8_t* combinedBitmap,
                         uint32_t totalWidthAllFaces,
                         uint32_t maxHeightAllFaces) {
  ofstream fastfont(
      GLWin::baseDir + "fast.glfont",
      ios::binary);  // save a binary file for rapid loading next time
  FastFontHeader header;
  header.magic = 0x544E4644;
  header.version = 1;
  header.w = totalWidthAllFaces;
  header.h = maxHeightAllFaces;
  header.numFaces = faces.size();
  header.numFonts = 32 * 3;  // TODO: fix this
  header.numGlyphs = 0;      // TODO: fix
  fastfont.write((char*)&header, sizeof(header));
  fastfont.write((char*)combinedBitmap, totalWidthAllFaces * maxHeightAllFaces);
  for (auto face : faces) {
    face->save(fastfont);  // save individual face
  }
}

/*
TODO: this just currently returns one default font, make it work!
*/
/* gets the font size */
const Font* FontFace::getFont(uint32_t size, int weight) const {
  std::unordered_map<uint32_t, uint32_t>::const_iterator index =
      fontBySize.find(size);
  if (index == fontBySize.end()) {
    // TODO::Load Font
    std::string msg = "Size: " + std::to_string(size);
    cerr << "cannot load font " << faceName << " size " << size << '\n';
    // throw Ex2(Errcode::FONT_SIZE_LOAD, msg.c_str());
    return fonts[0];  // TODO: perhaps give them the closest font to the request
                      // later
  } else
    return fonts[index->second];
}

/* gets the font family */
const Font* FontFace::get(const char faceName[], uint32_t size,
                          uint32_t boldness) {
  auto index = faceByName.find(faceName);
  if (index == faceByName.end()) {
    throw Ex2(Errcode::FONT_LOAD, faceName);
    // TODO: Load in the font, if not throw exception
  } else {
    //    cout << "font size " << index->first << "(" << size << ") is at index
    //    "
    //         << index->second << '\n';

    return faces[index->second]->getFont(size, boldness);
  }
}

// TODO: remove parameter size and get font paths from config file for
// portability
void FontFace::addFontName(std::string name, const std::string& path) {
  name.erase(name.end() - 4, name.end());
  pathByName[name] = path;
}

const char* FontFace::FONT_CONF_FILEPATH = "$GRAIL/conf/fonts.conf";
// bool FontFace::hasBeenInitialized = false;
void FontFace::initAll() {
  // if (hasBeenInitialized) return;
  // hasBeenInitialized = true;
  // TODO: load from config file
  std::string fontPath = GLWin::baseDir + "conf/fonts/";
  cout << "FONT PATH = " << fontPath << endl;

  if (FT_Init_FreeType(&FontFace::ftLib))
    throw Ex1(Errcode::INITIALIZE_FREETYPE);
  // Create a hashmap with the name and filepath of all fonts specified in
  // dirPaths
  string baseDir = GLWin::baseDir;
  string fontBase = baseDir + "conf/fonts/";
  ifstream fontConf(baseDir + "conf/fonts.conf");
  string faceName, facePath;
  uint32_t minFontSize, inc, maxFontSize;

  char lineBuf[4096];
  char sizeSpec[256];
  char minStr[256], incStr[256], maxStr[256];
  uint32_t sizeX, sizeY;
  fontConf >> sizeX >> sizeY;
  uint8_t* bitmap = new uint8_t[sizeX * sizeY];
  uint32_t currX = 0,
           currY = 0;  // starting location to put glyphs. We will move right to
                       // the end of each line, then down to the next scan line
  uint32_t rowSize = 0;  // tracks the largest vertical size placed on this
                         // line. For simplicity we skip that much space
  // TODO: better packing could be achieved
  memset(bitmap, 0, sizeX * sizeY);  // clear the entire bitmap
  while (fontConf.getline(lineBuf, sizeof(lineBuf))) {
    if (lineBuf[0] == '\0' || lineBuf[0] == '#')
      continue;  // quick hack to skip comments and blank lines (blank lines
                 // would work anyway, but this is cleaner)
    istringstream line(lineBuf);
    line >> faceName >> facePath >> sizeSpec;

    istringstream size(sizeSpec);
    size.getline(minStr, sizeof(minStr), ',');
    size.getline(incStr, sizeof(minStr), ',');
    size.getline(maxStr, sizeof(minStr), ',');
    minFontSize = atoi(minStr);
    inc = atoi(incStr);
    maxFontSize = atoi(maxStr);
    if (line) {
      new FontFace(ftLib, faceName, fontBase + facePath, minFontSize, inc,
                   maxFontSize, bitmap, sizeX, sizeY, currX, currY, rowSize);
    }
  }
  for (int i = 1400; i < sizeY; i++) {
    int r = i * sizeX;
    for (int j = 0; j < sizeX / 4; j += 2) {
      bitmap[r + j] = 255;
      bitmap[r + j + 1] = 0;
    }
    for (int j = sizeX / 4; j < sizeX / 2; j += 4) {
      bitmap[r + j] = 255;
      bitmap[r + j + 1] = 255;
      bitmap[r + j + 2] = 255;
      bitmap[r + j + 3] = 255;
    }
    for (int j = sizeX / 2; j < sizeX * 3 / 4; j += 8) {
      bitmap[r + j] = 255;
      bitmap[r + j + 1] = 0;
      bitmap[r + j + 2] = 0;
      bitmap[r + j + 3] = 0;
    }
    for (int j = sizeX * 3 / 4; j < sizeX; j++) {
      bitmap[r + j] = 255;
    }
  }
  //    bitmap[i] = 255;//bitmap[i-sizeX*1450];

  setTexture(bitmap, sizeX, sizeY);
  delete[] bitmap;
}

#pragma once

// #include <ft2build.h>

#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
// #include FT_FREETYPE_H
// #include FT_GLYPH_H

class GLWin;
class Font;
typedef struct FT_LibraryRec_  *FT_Library;
typedef struct FT_FaceRec_*  FT_Face;

class FontFace {
 private:
  friend class Font;
  //  static bool hasBeenInitialized;
  static const char* FONT_CONF_FILEPATH;
  static std::vector<std::string> loaded_fonts;
  std::string faceName;
  uint32_t boldness : 5;
  uint32_t italics : 5;
  uint32_t fixed : 1;
  uint32_t numGlyphs;  // the number of distinct glyphs rendered into this face

  // look up a font by size --> index into the fonts vector
  std::unordered_map<uint32_t, uint32_t> fontBySize;
  static std::unordered_map<std::string, uint32_t> faceByName;
  static std::vector<FontFace*> faces;
  std::vector<Font*> fonts;  // each font object is rendered for a specific size
  void addFont(Font* f) { fonts.push_back(f); }
  uint32_t textureId;  // id of the texture in OpenGL for the ENTIRE FONT FACE
  static std::unordered_map<std::string, std::string> pathByName;

  static void setTexture(const uint8_t bitmap[], uint32_t w, uint32_t h);

  void save(std::ostream& s);  // save a single font face to binary file

  // save all font faces to a fast binary file for instant retrieval later
  static void saveFonts(const uint8_t* combinedBitmap, uint32_t sizeX,
                        uint32_t sizeY);
  static bool combineFaces();

 public:
  uint32_t maxWidthIndex;
  static void initAll();
  static void addFontName(std::string name, const std::string& path);
  uint32_t getTexture() const { return textureId; }

  /* Build a font face by
      opening a freetype face
      generating each size
      storing the rendered characters into a single large bitmap in row-major
     scan order
  */
  FontFace(FT_Library ft, const std::string& faceName,
           const std::string& facePath, uint32_t minFontSize, uint32_t inc,
           uint32_t maxFontSize, uint8_t bitmap[], uint32_t& sizeX,
           uint32_t& sizeY, uint32_t& currX, uint32_t& currY,
           uint32_t& rowSize);
  static void emptyFaces();
  ~FontFace();
  static FT_Library ftLib;
  const Font* getFont(uint32_t size, int weight) const;

  static const FontFace* getFace(int i) { return faces.at(i); }
  static const Font* get(const char faceName[], uint32_t size,
                         uint32_t boldness);
  static constexpr char TIMES[] = "Times";
  static constexpr char HELVETICA[] = "Helvetica";
  static constexpr char COURIER[] = "Courier";
  static constexpr uint32_t NORMAL = 1;
  //	static constexpr uint32_t ITALICS = 9;

  static constexpr uint32_t BOLD = 9;
};

class Font {
  friend class FontFace;

 public:
  class Glyph {
   public:
    float advance;
    float bearingX, bearingY;
    float sizeX, sizeY;
    float u0;  // left side of texture, always zero for vertical strip font
    float u1;  // right side of texture, always 1.0 for vertical strip font
    float v1;  // top edge of texture
    float v0;  // bottom edge of texture
    // NOTE the order!!! v1 is top, v0 is bottom
    Glyph(float advance, float bearingX, float bearingY, float sizeX,
          float sizeY, float u0, float u1, float v1, float v0)
        : advance(advance),
          bearingX(bearingX),
          bearingY(bearingY),
          sizeX(sizeX),
          sizeY(sizeY),
          u0(u0),
          u1(u1),
          v1(v1),
          v0(v0) {}
    Glyph() {}  // TODO: uninitialized just for loading, better way?
    friend std::ostream& operator<<(std::ostream& s, const Font::Glyph& g);
  };

 private:
  FontFace* parentFace;
  // TODO:uint32_t parentFace;   // offset of parent into the static fontface
  // table for ease of serialization to disk
  uint16_t height;  // height of rectangle for each glyph of this font in pixels
  uint32_t
      startGlyph;  // first character to be written, 32 bits to handle any char
  // uint32_t numGlyphs;    // number of glyphs supported, some might be the
  // same
  uint32_t textureId;
  std::vector<Glyph> glyphs;

 public:
  uint32_t maxWidth;    // biggest width of any glyph
  uint16_t spaceWidth;  // width of a space character
  Font(FontFace* Face, FT_Face ftFace, uint16_t height, uint8_t bitmap[],
       uint32_t& sizeX, uint32_t& sizeY, uint32_t& currX, uint32_t& currY,
       uint32_t& rowSize);
  Font(std::istream&);
  ~Font();

  uint32_t getStartGlyph() const { return startGlyph; }
  void addGlyph(FT_Face ftFace,
                std::unordered_map<uint32_t, uint32_t>& glyphMap, uint8_t c,
                uint8_t bitmap[], uint32_t& sizeX, uint32_t& sizeY,
                uint32_t& currX, uint32_t& currY, uint32_t& rowSize);
  const Glyph* getGlyph(uint32_t i) const { return &glyphs.at(i); }
  uint16_t getHeight() const { return height; }
  uint32_t getTexture() const {
    return textureId;
  }  // get the texture shared by all Fonts within the FontFace
  float getWidth(const char text[], const uint32_t len) const;
  friend std::ostream& operator<<(std::ostream& s, const Font& f) {
    return s << "Font height=" << f.height << " numGlyphs=" << f.glyphs.size();
    // return s << "Font " << f.parentFace->faceName << " height=" << f.height
    // << " numGlyphs=" << f.numGlyphs; // friend isn't working? what a pain
    // return s << "Font " << FontFace::faces[f.parentFace]->faceName << "
    // height=" << f.height << " numGlyphs=" << f.numGlyphs;
  }
  void save(std::ostream& fastfont);
  static Font* getDefault();
};

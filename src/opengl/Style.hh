#pragma once

#include <glm/glm.hpp>

// temporary to fix compile issues
#include "opengl/GLWinFonts.hh"

class Style {
 public:
  const Font* f;
  glm::vec4 bg;
  glm::vec4 fg;
  float lineWidth;
  int shaderIndex;

 public:
  Style(const char* fontFamily, float fontSize, float fontWeight, float bgRed,
        float bgGreen, float bgBlue, float fgRed, float fgGreen, float fgBlue,
        int lineWidth = 1, int shaderIndex = 0)
      : f(lookup(fontFamily, fontSize, fontWeight)),
        bg(bgRed, bgGreen, bgBlue, 1),
        fg(fgRed, fgGreen, fgBlue, 1),
				lineWidth(lineWidth),
        shaderIndex(shaderIndex)
        {}

  Style(const char* fontFamily, float fontSize, float fontWeight, float bgRed,
        float bgGreen, float bgBlue, float bgAlpha, float fgRed, float fgGreen,
        float fgBlue, float fgAlpha, int lineWidth = 1, int shaderIndex = 0)
      : f(lookup(fontFamily, fontSize, fontWeight)),
        bg(bgRed, bgGreen, bgBlue, bgAlpha),
        fg(fgRed, fgGreen, fgBlue, fgAlpha),
        shaderIndex(shaderIndex),
        lineWidth(lineWidth) {}

  Style(const Font* font,
				float bgRed, float bgGreen, float bgBlue,
				float fgRed, float fgGreen, float fgBlue,
				int lineWidth = 1, int shaderIndex = 0)
      : f(font),
        bg(bgRed, bgGreen, bgBlue, 1),
        fg(fgRed, fgGreen, fgBlue, 1),
        shaderIndex(shaderIndex),
        lineWidth(lineWidth) {}
	
  Style(const Font* font, const glm::vec4& bgColor, const glm::vec4& fgColor, int lineWidth = 1)
      : f(font), bg(bgColor), fg(fgColor), lineWidth(lineWidth) {}

  const Font* lookup(const char* fontFamily, const float size,
                     const float weight) {
    return FontFace::get(fontFamily, size, weight);
  }

  void apply() {}
  void setShaderIndex(uint32_t val) { shaderIndex = val; }
  uint32_t getShaderIndex() const { return shaderIndex; }
  uint32_t getLineWidth() const { return lineWidth; }

  void setLineWidth(uint32_t val) { lineWidth = val; }
  const glm::vec4& getBgColor() const { return bg; }

  const glm::vec4& getFgColor() const { return fg; }

  static Style* getStyle(uint8_t a) { return nullptr; }
};

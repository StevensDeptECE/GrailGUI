#include "opengl/Shaders.hh"

/*
  This experimental code reads in all the standard shaders, generates binary shader programs, and writes them out into text files
  in C++ code as arrays in hex. The result can then be compiled, bypassing the need to 
  */
int main() {
  Shader::setDir(prefs.getShaderDir());
  Shader::load("solid.bin", "common.vert", "common.frag");  // Solid Color
  Shader::load("pervert.bin", "vColor.vert", "common.frag"); // Color per vertex
  Shader::load("text.bin", "text.vert", "text.frag");  // Texture for text
  Shader::load("img.bin", "Texture.vert", "Texture.frag");  // Texture for images
  Shader::load("cursor.bin", "Cursor.vert", "common.frag"); // Texture for images
  Shader::load("multiText.bin", "MultiTexture.vert", "MultiTexture.frag");  // MultiTexture for shapes
}
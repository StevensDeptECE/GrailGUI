#include "opengl/GrailGUI.hh"
/*
  Use GLWin to build a window (which is unnecessary for this program but easier)
  Then take the binaries from the shaders and save them out.
  This will necessitate adding a hook into the shader initialization in GLWin, but it is
  less maintenance than writing the same code in another place.

  Each shader will be extracted as a sequence of bytes, written to an array in hex, and placed
  in a directory src/precompiledshaders/shader_xxxx.cc

*/
void grailmain(int argc, char* argv[], GLWin* w, Tab* t) {
  w->setTitle("Precompiling shader test");

}


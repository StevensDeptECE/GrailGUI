#pragma once
#include "opengl/Canvas.hh"
#include "opengl/GLMath.hh"
#include "opengl/GLWin.hh"
#include "opengl/Member.hh"
#include "opengl/MultiText.hh"
#include "opengl/Style.hh"
#include "opengl/StyledMultiShape2D.hh"
#include "opengl/Tab.hh"
#include "util/Ex.hh"

void mainErrorHandling(const char title[], uint32_t width, uint32_t height,
  uint32_t fgColor, uint32_t bgColor, int argc, char* argv[]);

#pragma once

#include <cstdint>
#include <iostream>
#include "opengl/Errcode.hh"
#include "util/Ex.hh"
#include "util/Log.hh"
extern Log srvlog;

#ifdef __linux__
#define binFlags O_RDONLY
#elif _WIN32
#define binFlags O_RDONLY | O_BINARY
#endif

#pragma once

#include <cstdint>
#include <iostream>

#include "opengl/Errcode.hh"
#include "util/Ex.hh"
#include "util/Log.hh"
extern Log srvlog;

#ifdef __linux__
#define readBinFlags O_RDONLY
#elif _WIN32
#define readBinFlags O_RDONLY | O_BINARY
#endif

#ifdef __linux__
#define writeBinFlags O_WRONLY
#elif _WIN32
#define writeBinFlags O_WRONLY | O_BINARY
#endif
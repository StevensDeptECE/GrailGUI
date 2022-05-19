cmake_minimum_required(VERSION 3.16.1)

project(
  Grail
  VERSION 0.1.0
  DESCRIPTION "Grail"
  LANGUAGES C CXX)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)
set(CMAKE_FILES_DIRECTORY build)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)
set(CMAKE_CXX_STANDARD 20)
option(CMAKE_CXX_STANDARD_REQUIRED ON)
option(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON
    CACHE INTERNAL "")
option(CMAKE_LINK_WHAT_YOU_USE ON)
option(GRAIL_EXPERIMENTAL ON)
option(GRAIL_EXTRA_DEBUG_WARNINGS OFF)
option(GRAIL_WERROR OFF)
set(CMAKE_DEBUG_POSTFIX d)

string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type_lower)

# Set a default build type if none was specified
set(default_build_type "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS
      "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE
      "${default_build_type}"
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                               "MinSizeRel" "RelWithDebInfo")
endif()

# BUG: This will more than likely not behave how we want it to with
# multi-configuration generators https://stackoverflow.com/a/24470998
message(STATUS "Build mode: ${CMAKE_BUILD_TYPE}")
if(build_type_lower MATCHES "debug")
  message(STATUS "Building shared libs")
  option(BUILD_SHARED_LIBS ON)
else()

  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT supported OUTPUT error)

  if(supported)
    message(STATUS "IPO / LTO enabled")
    set(TARGET grail PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(STATUS "IPO / LTO not supported: <${error}>")
  endif()
endif()

if(GRAIL_EXTRA_DEBUG_WARNINGS)
  if(MSVC)
    # warning level 4
    add_compile_options(/W4)
  else()
    # lots of warnings
    add_compile_options(-Wall -Wextra)
  endif()
endif()

if(GRAIL_WERROR)
  # all warnings are errors
  if(MSVC)
    add_compile_options(/WX)
  else()
    add_compile_options(-Werror)
  endif()
endif()

include(FetchContent)
include(GrailFunctions)

find_package(PkgConfig REQUIRED)

# opengl
find_package(OpenGL REQUIRED)

# GLFW
find_package(glfw3 REQUIRED)

# Freetype
find_package(Freetype REQUIRED)

# MPV
find_package(MPV)

# GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  # 0.9.9.8 does not have C++ 20 fix yet, so master branch is currently used
  # GIT_TAG        bf71a834948186f4097caa076cd2663c69a10e1e # 0.9.9.8
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/glm)
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
  FetchContent_Populate(glm)
endif()

# stats.hh

FetchContent_Declare(
  stats
  GIT_REPOSITORY https://gitlab.com/Silent__Ninja346/stats
  # tag is normally set to origin/main for this, but is currently pinned
  GIT_TAG 888e8291099fc869e89b2520b25880bfe8acebf0
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/stats)
FetchContent_GetProperties(stats)
if(NOT stats_POPULATED)
  FetchContent_Populate(stats)
endif()

# fmtlib This will eventually be merged into C++20 as std::format, but for now
# this is using fmtlib
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG d141cdbeb0fb422a3fb7173b285fd38e0d1772dc # v8.0.1
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/external/fmt)
FetchContent_MakeAvailable(fmt)

# Shapelib Currently manually compiled due to compiler warnings Output is not
# shown, so errors may occur without notice option(FETCHCONTENT_QUIET "Enables
# QUIET option for all content population" ON) FetchContent_Declare( shapelib
# GIT_REPOSITORY https://github.com/OSGeo/shapelib.git #GIT_TAG       The tags
# have not been updated since 2019, so no tag SOURCE_DIR
# ${PROJECT_SOURCE_DIR}/external/shapelib ) FetchContent_MakeAvailable(shapelib)

add_subdirectory(proj)
add_subdirectory(include/libshape)
add_subdirectory(src)

target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/external/glm)
target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/external/stats)
target_include_directories(grail
                           PUBLIC ${PROJECT_SOURCE_DIR}/external/fmt/include)
target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/external)

target_include_directories(grailserver PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(grailserver PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(grailserver
                           PUBLIC ${PROJECT_SOURCE_DIR}/external/glm)
target_include_directories(grailserver
                           PUBLIC ${PROJECT_SOURCE_DIR}/external/fmt/include)
target_include_directories(grailserver PUBLIC ${PROJECT_SOURCE_DIR}/external)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  target_include_directories(grail PUBLIC /mingw64/include)
endif()

add_subdirectory(test)

dump_cmake_variables()

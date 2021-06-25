cmake_minimum_required(VERSION 3.16.1)

project(Grail VERSION 0.1.0
DESCRIPTION
"Grail"
LANGUAGES
C CXX)

set(CMAKE_BINARY_DIR bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH libs)
set(CMAKE_FILES_DIRECTORY build) 
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)
set(CMAKE_CXX_STANDARD 17)
option(CMAKE_CXX_STANDARD_REQUIRED ON)
option(CMAKE_CXX_EXTENSIONS OFF)
option(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(CMAKE_LINK_WHAT_YOU_USE ON)
set(GRAIL_EXPERIMENTAL "OFF")
set(CMAKE_DEBUG_POSTFIX d)

message(STATUS "test ${CMAKE_PREFIX_PATH}")
# opengl
find_package(OpenGL REQUIRED)

# GLFW
find_package(glfw3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW_INCLUDE_DIRS}")

# GLM
#find_package(GLM REQUIRED)
#include_directories(SYSTEM ${GLM_INCLUDE_DIRS})
#message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# Freetype
find_package(Freetype REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)


#if(CMAKE_SYSTEM MATCHES "Linux")
#	target_link_libraries()
#endif()

add_subdirectory(test)

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
set(CMAKE_CXX_STANDARD 17)
option(CMAKE_CXX_STANDARD_REQUIRED ON)
option(CMAKE_CXX_EXTENSIONS OFF)
option(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(CMAKE_LINK_WHAT_YOU_USE ON)
set(GRAIL_EXPERIMENTAL "OFF")
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_BUILD_TYPE DEBUG)


add_subdirectory(src)

target_include_directories(grail PUBLIC src)
target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/include)

#if(CMAKE_SYSTEM MATCHES "Linux")
#	target_link_libraries()
#endif()

add_subdirectory(test)

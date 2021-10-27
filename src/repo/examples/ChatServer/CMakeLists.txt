cmake_minimum_required(VERSION 3.16.3)
project(chatserver)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES chatserver.cpp)

add_executable(chatserver chatserver.cpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(chatserver OpenSSL::SSL)
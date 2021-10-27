cmake_minimum_required(VERSION 3.16.3)
project(chatclient)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES chatclient.cpp)

add_executable(chatclient chatclient.cpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(chatclient OpenSSL::SSL)
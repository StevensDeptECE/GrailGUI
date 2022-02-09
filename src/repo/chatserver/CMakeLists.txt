cmake_minimum_required(VERSION 3.16.3)
project(chatserver2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES chatserver2.cpp)

add_executable(chatserver2 chatserver2.cpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(chatserver2 OpenSSL::SSL)
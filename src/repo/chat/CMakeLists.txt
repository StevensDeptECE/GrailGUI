cmake_minimum_required(VERSION 3.16.3)
project(chat)

set(CMAKE_CXX_STANDARD 11)

add_executable(chatclient chatclient.cpp)
add_executable(chatserver chatserver.cpp)

find_package(OpenSSL REQUIRED)
target_link_libraries(chatclient OpenSSL::SSL)
target_link_libraries(chatserver OpenSSL::SSL)

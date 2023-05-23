cmake_minimum_required (VERSION 3.26)
project (portable_networking)
set(CMAKE_BUILD_TYPE Debug)

# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_library (portable_networking csp/Socket.cc csp/IPV4Socket.cc util/Errcode.cc util/Buffer.cc csp/CSPRequest.cc csp/SocketIO.cc csp/Request.cc csp/CSPServlet.cc)
target_include_directories (portable_networking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
if (CMAKE_HOST_WIN32)
target_link_libraries(portable_networking ws2_32)
endif()
  

# Link to the common dependencies into a library
add_executable (client client.cc)
target_link_libraries (client LINK_PUBLIC portable_networking)

add_executable (server server.cc)
target_link_libraries (server LINK_PUBLIC portable_networking)

set(grail-chat
    ssl_socket.cc
    server.cc
    client.cc
)

list(TRANSFORM grail-chat PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-chat ${grail-chat} PARENT_SCOPE)
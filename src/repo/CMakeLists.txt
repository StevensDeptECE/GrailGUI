set(grail-repo
    stegano/Steganography.cc
    stegano/base64.cpp
    cloud/Cloud.cc
    ssl/SSLSocket.cc
    aes/AES.cc
    Repository.cc
)

list(TRANSFORM grail-repo PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-repo ${grail-repo} PARENT_SCOPE)

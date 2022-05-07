set(grail-repo
    stegano/steganography.cc
    cloud/cloud.cc
    template.cc
    chat
)

list(TRANSFORM grail-repo PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-repo ${grail-repo} PARENT_SCOPE)

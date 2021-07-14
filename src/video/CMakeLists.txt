set(grail-video
    VideoPlayer.cc
)

list(TRANSFORM grail-video PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-video ${grail-video} PARENT_SCOPE)

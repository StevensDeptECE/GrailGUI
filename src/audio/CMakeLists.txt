set(grail-audio
    AudioPlayer.cc
)

list(TRANSFORM grail-audio PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-audio ${grail-audio} PARENT_SCOPE)

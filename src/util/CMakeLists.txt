set(grail-util Buffer.cc Callbacks.cc datatype1.cc Prefs.cc
               Timers.cc)

list(TRANSFORM grail-util PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-util
    ${grail-util}
    PARENT_SCOPE)

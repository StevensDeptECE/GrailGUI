set(grail-repo 
    template.cc
)

list(TRANSFORM grail-repo PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-repo ${grail-repo} PARENT_SCOPE)

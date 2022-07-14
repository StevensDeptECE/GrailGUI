set(grail-maps
    Geometry.cc
    MapView2D.cc
    MapViewer.cc)

list(TRANSFORM grail-maps PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

set(grail-maps
    ${grail-maps}
    PARENT_SCOPE)

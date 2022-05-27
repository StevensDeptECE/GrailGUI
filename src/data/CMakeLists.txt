set(grail-data 
    BlockLoader2.cc
    BlockMapLoader2.cc
    BlockMapLoaderConverters2.cc 
    GapMinderBinaryDB.cc
    GapMinderLoader.cc
)

list(TRANSFORM grail-data PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-data ${grail-data} PARENT_SCOPE)
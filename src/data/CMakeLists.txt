set(grail-data 
    BlockMapLoader.cc
    BlockMapLoaderConverters.cc 
    GapMinderBinaryDB.cc
    GapMinderLoader.cc
)

list(TRANSFORM grail-data PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-data ${grail-data} PARENT_SCOPE)
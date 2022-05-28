# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

add_grail_executable(SRC convertESRItoBlockLoader.cc LIBS grail)
# add_grail_executable(SRC loadESRIShapefile.cc LIBS grail)
# add_grail_executable(SRC testESRIPolygon.cc LIBS grail)
add_grail_executable(SRC testDrawBlockMap.cc LIBS grail)
# add_grail_executable(SRC ESRIMapDemo.cc LIBS grail) 
add_grail_executable(SRC testLoadFromESRI.cc LIBS grail)
add_grail_executable(SRC testFastMapLoad.cc LIBS grail)
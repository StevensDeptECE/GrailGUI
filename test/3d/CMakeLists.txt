# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

add_grail_executable(SRC Globe.cc LIBS grail)
add_grail_executable(SRC SolarSystem.cc LIBS grail)
# add_grail_executable(SRC testDemo3D.cc LIBS grail)
# add_grail_executable(SRC testDrawing3D.cc LIBS grail
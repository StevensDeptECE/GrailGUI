# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

add_grail_executable(SRC testfmt.cc LIBS grail)
add_grail_executable(SRC testMultiTab.cc LIBS grail)
add_grail_executable(SRC testBoard.cc LIBS grail)
#add_grail_executable(SRC testVisualBoard.cc)
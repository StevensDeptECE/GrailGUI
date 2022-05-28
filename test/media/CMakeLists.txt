# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

if(${MPV_FOUND} AND ${PC_MPV_VERSION} VERSION_GREATER "1.26")
  add_grail_executable(SRC testSingleAudio.cc LIBS grail)
  add_grail_executable(SRC testAudio.cc LIBS grail)
  add_grail_executable(SRC testVideo.cc LIBS grail)
endif()
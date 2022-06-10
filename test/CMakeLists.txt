
# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

# 2D Graphics
add_subdirectory(2d)

# 3D Graphics
add_subdirectory(3d)

# CAD
# add_grail_executable(SRC CAD/CurveTest.cc LIBS grail)
# add_grail_executable(SRC CAD/HelixTest.cc LIBS grail)
# add_grail_executable(SRC CAD/BezierTest.cc LIBS grail)
# add_grail_executable(SRC CAD/Vec3DTest.cc LIBS grail)
# add_grail_executable(SRC CAD/CircleTest.cc LIBS grail)
# add_grail_executable(SRC CAD/ExtrusionTest.cc LIBS grail)
# add_grail_executable(SRC CAD/FacetTest.cc LIBS grail)
# add_grail_executable(SRC CAD/LineTest.cc LIBS grail)
# add_grail_executable(SRC CAD/LinkedPathTest.cc LIBS grail)
# add_grail_executable(SRC CAD/CurveTest.cc LIBS grail)
# add_grail_executable(SRC CAD/testCylinder.cc LIBS grail)
# add_grail_executable(SRC CAD/testSphere.cc LIBS grail)
# add_grail_executable(SRC CAD/testRectangularPrism.cc LIBS grail)
# add_grail_executable(SRC CAD/testTriangle.cc LIBS grail)
# add_grail_executable(SRC CAD/testRectangle.cc LIBS grail)
# add_grail_executable(SRC CAD/testCircle.cc LIBS grail)
# add_grail_executable(SRC CAD/testGear.cc LIBS grail)
# add_grail_executable(SRC CAD/testTransform.cc LIBS grail)

# Graphs
add_subdirectory(graph)

# GUI
# add_grail_executable(SRC GUI/testWidgets.cc LIBS grail)

# Maps
add_grail_executable(SRC maps/convertESRItoBlockLoader.cc LIBS grail)
add_grail_executable(SRC maps/loadESRIShapefile.cc LIBS grail)
# add_grail_executable(SRC maps/testESRIPolygon.cc LIBS grail)
add_grail_executable(SRC maps/testDrawBlockMap.cc LIBS grail)
# add_grail_executable(SRC maps/ESRIMapDemo.cc LIBS grail) 
add_grail_executable(SRC maps/testLoadFromESRI.cc LIBS grail)
add_grail_executable(SRC maps/testFastMapLoad.cc LIBS grail)



# Media
if(${MPV_FOUND} AND ${PC_MPV_VERSION} VERSION_GREATER "1.26")
  add_grail_executable(SRC media/testSingleAudio.cc LIBS grail)
  add_grail_executable(SRC media/testAudio.cc LIBS grail)
  add_grail_executable(SRC media/testVideo.cc LIBS grail)
endif()

# Networking

# Solar System
# add_grail_executable(BINNAME testSolar SRC solarsystem/DrawNASAEphemerisSolarSystem2d.cc LIBS grail)


# XDL
# add_grail_executable(SRC xdl/testStockServer.cc LIBS grail)
add_grail_executable(SRC xdl/testXDLButton.cc LIBS grail)

# XP (Experimental)
add_grail_executable(SRC xp/testfmt.cc LIBS grail)
add_grail_executable(SRC xp/testMultiTab.cc LIBS grail)
add_grail_executable(SRC xp/testHashMap.cc LIBS grail)

# Unsorted 
# add_grail_executable(SRC ../src/xp/decompressBlockLoader.cc LIBS grail lzma)
add_grail_executable(SRC ../src/csp/GenericXDLClient.cc LIBS grail)
add_grail_executable(SRC ../src/csp/SimpleXDLServer.cc LIBS grail)
# add_grail_executable(SRC ../src/csp/GraphicalXDLClient.cc LIBS grailserver)
# add_grail_executable(SRC testPopupMenu.cc LIBS grail) 
# add_grail_executable(SRC StockDemo.cc LIBS grail)







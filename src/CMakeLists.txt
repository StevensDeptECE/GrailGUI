add_library(grail data/BlockMapLoader.cc data/BlockMapLoaderConverters.cc
opengl/AngledMultiText.cc opengl/AxisWidget.cc opengl/BarChartWidget.cc 
opengl/BoxChartWidget.cc opengl/ButtonWidget.cc opengl/CandlestickChartWidget.cc 
opengl/Canvas.cc opengl/ErrNames.cc opengl/ESRIPolygon.cc opengl/ESRIShape.cc 
opengl/GapMinderWidget.cc opengl/GLWin.cc opengl/GLWinFonts.cc 
opengl/LineGraphWidget.cc opengl/MapView2D.cc opengl/MultiShape.cc 
opengl/MultiShape2D.cc opengl/MultiShape3D.cc opengl/ScrollbarWidget.cc 
opengl/Shader.cc opengl/Shape.cc opengl/Shape2D.cc opengl/Shapefile.cc 
opengl/StyledMultiShape2D.cc csp/IPV4Socket.cc csp/Request.cc csp/Socket.cc 
csp/SocketIO.cc csp/XDLRequest.cc util/Buffer.cc util/datatype.cc 
util/HashMap.cc xdl/std.cc xdl/SymbolTable.cc xdl/XDLCompiler.cc)


if(${GRAIL_EXPERIMENTAL})
add_subdirectory(xp)
endif()


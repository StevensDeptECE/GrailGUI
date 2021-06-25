set(grail-opengl
		AngledMultiText.cc
		AxisWidget.cc
		BarChartWidget.cc 
		BoxChartWidget.cc
		ButtonWidget.cc
		CandlestickChartWidget.cc 
		Canvas.cc
		ErrNames.cc
		ESRIPolygon.cc
		ESRIShape.cc 
		GapMinderWidget.cc
		GLWin.cc
		GLWinFonts.cc 
		LineGraphWidget.cc
		MapView2D.cc
		MultiShape.cc 
		MultiShape2D.cc
		MultiShape3D.cc
		ScrollbarWidget.cc 
		Shader.cc
		Shape.cc
		Shape2D.cc
		Shapefile.cc 
		StyledMultiShape2D.cc
)

list(TRANSFORM grail-opengl PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)
set(grail-opengl ${grail-opengl} PARENT_SCOPE)
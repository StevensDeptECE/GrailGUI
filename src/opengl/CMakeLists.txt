set(grail-opengl
    AxisWidget.cc
    BarChartWidget.cc
    BoxChartWidget.cc
    ButtonWidget.cc
    CandlestickChartWidget.cc
    Canvas.cc
    Colors.cc
    Document.cc
    DocView.cc
    ErrNames.cc
    ESRIPolygon.cc
    ESRIShape.cc
#    GapMinderWidget.cc
    GraphWidget.cc
    GLWin.cc
    GLWinFonts.cc
    Image.cc
    LineGraphWidget.cc
    Member.cc
    MultiShape.cc
    MultiShape2D.cc
    MultiShape3D.cc
    MultiText.cc
    MultiText2.cc
    NavigationBar.cc
    PopupMenu.cc
    Scale.cc
    ScrollbarWidget.cc
    Shader.cc
    Shape.cc
    Shape2D.cc
    Shapefile.cc
    SparklineWidget.cc
    StyledMultiShape2D.cc
    Tab.cc
    util/TextureArray.cc
    PositionTool.cc)

list(TRANSFORM grail-opengl PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/)

set(grail-opengl
    ${grail-opengl}
    PARENT_SCOPE)

include ${GRAIL}/proj/settings.mk
# Brief explanation of syntax:--------------------------------------------------------------------------------------

# $@ = the first target before ':'
# $^ = the entire list of dependencies for the target
# $(CXX) $^ -o $@ $(LIBS) = compile all listed dependencies with the libs, into the target ($@ = $(BIN)/something)
#-------------------------------------------------------------------------------------------------------------------

#OBJ	:= ../src/grail/CompiledWebWindow.o ../src/grail/ErrNames.o

all: $(BIN)/testAudio # $(BIN)/testWidgets $(BIN)/testAxis $(BIN)/testLineGraph $(BIN)/testImage\
 $(BIN)/simpleDemo  $(BIN)/testDisplayBook $(BIN)/testGrid $(BIN)/JoeyDemo  \
 $(BIN)/testStyledMultishape $(BIN)/testText3 $(BIN)/globe  $(BIN)/SolarSystem \
 $(BIN)/convertASCIITable $(BIN)/testDisplayEntireBook \
 $(BIN)/LuisaDemo $(BIN)/QuentinDemo $(BIN)/MapDemo $(BIN)/StockDemo

#$(BIN)/testText 
#$(BIN)/testDrawing
#$(BIN)/testGraphViewer
#$(BIN)/testDisplayEntireBook

#$(BIN)/testDrawing3D $(BIN)/testDemo3D
#$(BIN)/testHeatMap testHeatMap does not yet work, so commented out
#$(BIN)/testCompletePoly 
#$(BIN)/convertASCIITable #$(BIN)/testJulian #$(BIN)/testColorfulCanvases  
#$(BIN)/testLoadpage $(BIN)/testCandleStick $(BIN)/testGraph $(BIN)/testAnim $(BIN)/testCompletePoly
#all: $(BIN)/testDisplayBook $(BIN)/simpleDemo 

#$(BIN)/testGraph ActionListener.o $(BIN)/testCursors3
#plot.o  BarChart.o JulianDate.o TimeSeries.o $(BIN)/test7 $(BIN)/test5 $(BIN)/testText $(BIN)/testJulian $(BIN)/test8 $(BIN)/testTimeSeries $(BIN)/testBin 

$(BIN)/testGrid: testGrid.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/simpleDemo: simpleDemo.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/StockDemo: StockDemo.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testGenericTable: testGenericTable.o $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/JoeyDemo: JoeyDemo.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testCompletePoly: testCompletePoly.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testDisplayBook: testDisplayBook.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testDisplayEntireBook: testDisplayEntireBook.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testDrawing: testDrawing.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testDrawing3D: testDrawing3D.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testDemo3D: testDemo3D.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/QuentinDemo: QuentinDemo.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)	

$(BIN)/MapDemo: MapDemo.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)	


$(BIN)/SolarSystem: SolarSystem.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/globe: Globe.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testStyledMultishape: testStyledMultishape.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)


$(BIN)/LuisaDemo: LuisaDemo.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testWidgets: testWidgets.cc $(LIBGRAIL)
	$(CXX) $^  -o $@ $(LIBS)

$(BIN)/testAxis: testAxis.cc $(LIBGRAIL)
	$(CXX) $^  -o $@ $(LIBS)

$(BIN)/testLineGraph: testLineGraph.cc $(LIBGRAIL)
	$(CXX) $^  -o $@ $(LIBS)

$(BIN)/testAudio: testAudio.cc $(LIBGRAIL)
	$(CXX) $^  -o $@ $(LIBS)

$(BIN)/testHeatMap: testHeatMap.cc $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testTable2: testTable2.cc $(LIBGRAIL)
	$(CXX) testTable2.cc -o $(BIN)/testTable2 $(LIBS)

$(BIN)/convertASCIITable: convertASCIITable.cc QuoteTable.cc  QuoteTable.hh $(utilout)/Buffer.o $(LIBGRAIL)
	$(CXX) convertASCIITable.cc QuoteTable.cc $(utilout)/Buffer.o -o $(BIN)/convertASCIITable $(LIBS)

$(BIN)/testBin: testBin.cc $(LIBGRAIL)
	$(CXX) testBin.cc -o $(BIN)/testBin $(LIBS)

$(BIN)/testTimeSeries: testTimeSeries.cc $(LIBGRAIL) TimeSeries.o JulianDate.o
	$(CXX) $^  -o $(BIN)/testTimeSeries TimeSeries.o JulianDate.o $(LIBS)

$(BIN)/testCandleStick: testCandleStick.cc $(LIBGRAIL) TimeSeries.o JulianDate.o
	$(CXX) $^  -o $(BIN)/testCandleStick TimeSeries.o JulianDate.o $(LIBS)

$(BIN)/testCursors: testCursors.cc $(LIBGRAIL)
	$(CXX) $^ -o $(BIN)/testCursors $(LIBS)

$(BIN)/testCursors2: testCursors2.cc $(LIBGRAIL) ActionListener.o
	$(CXX) $^ -o $(BIN)/testCursors2 ActionListener.o $(LIBS)

$(BIN)/testCursors3: testCursors3.cc $(LIBGRAIL) ActionListener.o
	$(CXX) $^ -o $(BIN)/testCursors3 ActionListener.o $(LIBS)

$(BIN)/testLoadpage: testLoadpage.cc $(LIBGRAIL)
	$(CXX) $^ -o $(BIN)/testLoadpage $(LIBS)

$(BIN)/testProjection: testProjection.cc $(LIBGRAIL)
	$(CXX) $^ -o $(BIN)/testProjection $(LIBS)

$(BIN)/testImage: testImage.cc $(LIBGRAIL)
	$(CXX) $^ -o $(BIN)/testImage $(LIBS)

# create an n x n grid of colorful canvases
$(BIN)/testColorfulCanvases:	testColorfulCanvases.cc $(LIBGRAIL)
	$(CXX) $^  -o $(BIN)/testColorfulCanvases $(LIBS)

# create n x n grid of rectangles using a single StyledMultiShape2D
# this is much lighter weight than separate canvases, but passing in styles is still a pain.
# we need something better?
$(BIN)/testRectangles:	testRectangles.cc $(LIBGRAIL)
	$(CXX) $^  -o $(BIN)/testRectangles $(LIBS)

# The beast that is so hard: displaying a line of text
$(BIN)/testText: testText.cc $(LIBGRAIL)
	$(CXX) $^  -o $(BIN)/testText $(LIBS)

# The beast that is so hard: displaying a line of text
$(BIN)/testText2: testText2.cc $(LIBGRAIL)
	$(CXX) $^  -o $(BIN)/testText2 $(LIBS)

# The beast that is so hard: displaying a line of text
$(BIN)/testText3: testText3.cc $(LIBGRAIL)
	$(CXX) $^  -o $(BIN)/testText3 $(LIBS)

$(BIN)/testTable: testTable.cc $(LIBGRAIL)
	$(CXX) $^  -o $(BIN)/testTable  $(LIBS)

$(BIN)/testJulian: testJulian.cc $(LIBGRAIL)
	$(CXX) $^  -o $(BIN)/testJulian  $(LIBS)


ActionListener.o: ActionListener.hh ActionListener.cc
	$(CXX) -c ActionListener.cc

plot.o:	plot.hh plot.cc $(LIBDIR)/libgrail.a
	$(CXX) -c plot.cc

BarChart.o: BarChart.hh BarChart.cc $(LIBDIR)/libgrail.a
	$(CXX) -c BarChart.cc

JulianDate.o: ../src/util/JulianDate.hh ../src/util/JulianDate.cc $(LIBDIR)/libgrail.a
	$(CXX) -c ../src/util/JulianDate.cc

TimeSeries.o: TimeSeries.hh TimeSeries.cc JulianDate.o $(LIBDIR)/libgrail.a
	$(CXX) -c TimeSeries.cc

$(BIN)/testGraphViewer: testGraphViewer.cc ${SRC}/visualcs/CSRGraph.hh $(opengl)/primitivesV2/GraphView.cc $(opengl)/primitivesV2/GraphView.hh 
	$(CXX)  testGraphViewer.cc $(opengl)/primitivesV2/GraphView.cc -o $(BIN)/testGraphViewer  $(LIBS)

$(BIN)/testAnim: testAnim.cc ${SRC}/visualcs/CSRGraph.hh $(opengl)/primitivesV2/GraphView.cc $(opengl)/primitivesV2/GraphView.hh $(LIBDIR)/libgrail.a
	$(CXX)  testAnim.cc $(opengl)/primitivesV2/GraphView.cc -o $(BIN)/testAnim $(LIBS)


$(BIN)/testJulianDate: testJulianDate.cc $(LIBDIR)/libgrail.a
	$(CXX)  testJulianDate.cc -o $(BIN)/testAnim  $(LIBS)

clean:
	rm -f bin/test*

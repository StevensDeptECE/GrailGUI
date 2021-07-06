include ${GRAIL}/proj/settings.mk
# Brief explanation of syntax:--------------------------------------------------------------------------------------

# $@ = the first target before ':'
# $^ = the entire list of dependencies for the target
# $(CXX) $^ -o $@ $(LIBS) = compile all listed dependencies with the libs, into the target ($@ = $(BIN)/something)
#-------------------------------------------------------------------------------------------------------------------

#OBJ	:= ../src/grail/CompiledWebWindow.o ../src/grail/ErrNames.o

all: $(BIN)/testGapMinder $(BIN)/testGapMinderLoader $(BIN)/testWidgetGapMinder #$(BIN)/testBlockMapLoader  $(BIN)/testFastMapLoad 

$(BIN)/testDrawBlockMap: testDrawBlockMap.cc $(data)/BlockMapLoader.cc $(opengl)/MapView2D.cc $(LIBGRAIL) $(opengl)/MapView2D.hh $(data)/BlockMapLoader.hh $(data)/BlockLoader.hh 
	$(CXX) $< $(data)/BlockMapLoader.cc $(opengl)/MapView2D.cc -o $@ $(LIBS)

$(BIN)/testBlockMapLoader: testBlockMapLoader.cc $(data)/BlockMapLoader.cc $(data)/BlockMapLoaderConverters.cc $(LIBGRAIL) $(data)/BlockMapLoader.hh $(data)/BlockLoader.hh 
	$(CXX) $< $(data)/BlockMapLoader.cc $(data)/BlockMapLoaderConverters.cc -o $@ $(LIBS)

$(BIN)/testFastMapLoad: testFastMapLoad.cc $(data)/BlockMapLoader.cc $(data)/BlockMapLoaderConverters.cc $(LIBGRAIL) $(data)/BlockMapLoader.hh $(data)/BlockLoader.hh 
	$(CXX)  $< $(data)/BlockMapLoader.cc $(data)/BlockMapLoaderConverters.cc -o $@ $(LIBS)

$(BIN)/testGapMinder: testGapMinder.cc $(data)/GapMinderBinaryDB.hh $(data)/GapMinderBinaryDB.cc $(data)/Dataset.hh
	$(CXX)  $< $(data)/GapMinderBinaryDB.cc -o  $@

$(BIN)/testGapMinderLoader: testGapMinderLoader.cc $(data)/GapMinderLoader.hh $(data)/GapMinderLoader.cc $(data)/BlockMapLoader.cc $(data)/Dataset.hh
	$(CXX)  $< $(data)/GapMinderLoader.cc $(data)/BlockMapLoader.cc -o  $@

$(BIN)/testWidgetGapMinder: testWidgetGapMinder.cc $(data)/GapMinderLoader.hh $(data)/GapMinderLoader.cc $(data)/BlockMapLoader.cc $(data)/Dataset.hh $(opengl)/GLWin.cc $(opengl)/GapMinderWidget.hh $(opengl)/GapMinderWidget.cc
	$(CXX)  $< $(data)/GapMinderLoader.cc $(data)/BlockMapLoader.cc $(opengl)/GapMinderWidget.cc -o  $@ $(LIBS)


clean:
	rm -f build/data/*


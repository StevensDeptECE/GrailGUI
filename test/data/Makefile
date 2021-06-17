include ${GRAIL}/proj/settings.mk
# Brief explanation of syntax:--------------------------------------------------------------------------------------

# $@ = the first target before ':'
# $^ = the entire list of dependencies for the target
# $(CXX) $^ -o $@ $(LIBS) = compile all listed dependencies with the libs, into the target ($@ = $(BIN)/something)
#-------------------------------------------------------------------------------------------------------------------

#OBJ	:= ../src/grail/CompiledWebWindow.o ../src/grail/ErrNames.o

all:    #$(BIN)/testBlockMapLoader  $(BIN)/testFastMapLoad 

$(BIN)/testDrawBlockMap: testDrawBlockMap.cc $(data)/BlockMapLoader.cc $(opengl)/MapView2D.cc $(LIBGRAIL) $(opengl)/MapView2D.hh $(data)/BlockMapLoader.hh $(data)/BlockLoader.hh 
	$(CXX) $< $(data)/BlockMapLoader.cc $(opengl)/MapView2D.cc -o $@ $(LIBS)

$(BIN)/testBlockMapLoader: testBlockMapLoader.cc $(data)/BlockMapLoader.cc $(data)/BlockMapLoaderConverters.cc $(LIBGRAIL) $(data)/BlockMapLoader.hh $(data)/BlockLoader.hh 
	$(CXX) $< $(data)/BlockMapLoader.cc $(data)/BlockMapLoaderConverters.cc -o $@ $(LIBS)

$(BIN)/testFastMapLoad: testFastMapLoad.cc $(data)/BlockMapLoader.cc $(data)/BlockMapLoaderConverters.cc $(LIBGRAIL) $(data)/BlockMapLoader.hh $(data)/BlockLoader.hh 
	$(CXX)  $< $(data)/BlockMapLoader.cc $(data)/BlockMapLoaderConverters.cc -o $@ $(LIBS)


clean:
	rm -f build/data/*

	
include ${GRAIL}/proj/settings.mk
# Brief explanation of syntax:--------------------------------------------------------------------------------------

# $@ = the first target before ':'
# $^ = the entire list of dependencies for the target
# $(CXX) $^ -o $@ $(LIBS) = compile all listed dependencies with the libs, into the target ($@ = $(BIN)/something)
#-------------------------------------------------------------------------------------------------------------------

#OBJ	:= ../src/grail/CompiledWebWindow.o ../src/grail/ErrNames.o

all: $(BIN)/testHashMap2

$(BIN)/testHashMap: testHashMap.cc $(util)/HashMap.hh
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testHashMap2: testHashMap2.cc $(util)/HashMap.hh $(util)/HashMap.cc
	$(CXX) $< -o $@ $(LIBS)

  


include ${GRAIL}/proj/settings.mk
# Brief explanation of syntax:--------------------------------------------------------------------------------------

# $@ = the first target before ':'
# $^ = the entire list of dependencies for the target
# $(CXX) $^ -o $@ $(LIBS) = compile all listed dependencies with the libs, into the target ($@ = $(BIN)/something)
#-------------------------------------------------------------------------------------------------------------------

#OBJ	:= ../src/grail/CompiledWebWindow.o ../src/grail/ErrNames.o

all: $(BIN)/testStats

$(BIN)/testStats: $(test)/testStats.cc Stats.hh
	$(CXX) $^  -o $@ $(LIBS)

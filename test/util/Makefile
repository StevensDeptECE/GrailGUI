include ${GRAIL}/proj/settings.mk
# Brief explanation of syntax:--------------------------------------------------------------------------------------

# $@ = the first target before ':'
# $^ = the entire list of dependencies for the target
# $(CXX) $^ -o $@ $(LIBS) = compile all listed dependencies with the libs, into the target ($@ = $(BIN)/something)
#-------------------------------------------------------------------------------------------------------------------

#OBJ	:= ../src/grail/CompiledWebWindow.o ../src/grail/ErrNames.o

all: $(BIN)/DateDemo #$(BIN)/testHashMap2 $(BIN)/testDate

$(BIN)/DateDemo: DateDemo.cc $(util)/HashMap.hh $(util)/JulianDate.cc
	$(CXX) $< $(util)/JulianDate.cc -o $@ $(LIBS)

$(BIN)/testHashMap: testHashMap.cc $(util)/HashMap.hh
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testHashMap2: testHashMap2.cc $(util)/HashMap.hh $(util)/HashMap.cc
	$(CXX) $< -o $@ $(LIBS)

$(BIN)/testDate: testDate.cc $(util)/JulianDate.hh $(util)/JulianDate.cc $(util)/Date.hh $(util)/Date.cc
	$(CXX) $< -o $@ $(LIBS) $(util)/JulianDate.cc $(util)/Date.cc

  

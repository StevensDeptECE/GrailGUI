include ${GRAIL}/proj/settings.mk
out=$(build)/util
CXX := g++ -g -O2 --std=c++14 -march=native -I.. -I$(out)
all: none

none:
	@echo "This Makefile is designed to be called from other Makefiles"

include ${GRAIL}/proj/common.mk
VPATH = $(util) $(out)

testhashmap2: testhashmap2.cc
#-fprofile-generate
#	g++ -g -O3 -march=native -mtune=native -fomit-frame-pointer testhashmap2.cc -o testhashmap2
#	g++ -O2 -march=native -fomit-frame-pointer -S testhashmap2.cc
#	g++ -g -O3 -march=native -mtune=native -fsanitize=address testhashmap2.cc -o testhashmap2
#	g++ -g -O3 -march=native -mtune=native testhashmap2.cc -o testhashmap2

JULDATE := testJulianDate.o JulianDate.o
$(BIN)/testjuliandate: $(JULDATE)
	$(CXX) -O3  $(JULDATE) -o testjuliandate

$(out)/testJulianDate.o: testJulianDate.cc JulianDate.hh
	$(CXX) -c $< -o $@

$(out)/JulianDate.o: JulianDate.cc JulianDate.hh
	$(CXX) -c $< -o $@

$(out)/HashMap.o: HashMap.cc HashMap.hh
	$(CXX) -c $< -o $@

$(out)/Buffer.o : Buffer.cc Buffer.hh
	$(COMP) $< -o $@

$(out)/datatype.o: datatype.cc datatype.hh
	$(CXX) -c $< -o $@
	
#testDynArray: testDynArray.cc DynArray.hh
#	g++ -g -I .. testDynArray.cc -o testDynArray

#testRCString: RCString.cc testRCString.cc RCString.hh
#	g++ -g -I .. RCString.cc testRCString.cc -o testRCString

clean:
	rm $(out)/*
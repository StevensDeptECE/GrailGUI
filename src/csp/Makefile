include ${GRAIL}/proj/settings.mk

out	:=	$(cspout)
VPATH = $(csp) $(out)

XDLSERVEROBJ	:=	$(out)/SimpleXDLServer.o $(out)/XDLRequest.o $(out)/Request.o $(out)/XDLCompiler.o )

all: init $(BIN)/simplexdlserver $(BIN)/simplexdlclient $(BIN)/genericxdlclient

include ${GRAIL}/proj/common.mk

$(BIN)/simplexdlserver: SimpleXDLServer.cc IPV4Socket.hh XDLRequest.hh $(LIBGRAIL)
	$(CXX) $< -o $@ $(LIBS)

#! Very bad -- this should be compiled in /src/xdl/Makefile, and then that Makefile referenced from here. FIX
#! Additionally, shouldn't go in cspout, but rather xdlout (might not be created rn)
#! Makefile in xdl is completely out of date... essentially take its targets and give it a full upgrade
#TODO: ^^

$(out)/XDLCompiler.o: $(xdl)/XDLCompiler.cc $(xdl)/XDLCompiler.hh
	$(COMP) $< -o $@

$(out)/SymbolTable.o: $(xdl)/SymbolTable.cc $(xdl)/SymbolTable.hh $(util)/Buffer.hh
	$(COMP) $< -o $@

$(out)/std.o: $(xdl)/std.cc $(xdl)/std.hh
	$(COMP) $< -o $@

#!------ End Bad Code -----------------------------------------------------------------------------------------

$(out)/IPV4Socket.o: IPV4Socket.cc IPV4Socket.hh Socket.hh
	$(COMP) $< -o $@

$(utilout)/Buffer.o: $(util)/Makefile $(util)/Buffer.cc $(util)/Buffer.hh
	cd $(util) && make $@

$(utilout)/datatype.o: $(util)/Makefile
	cd $(util) && make $@

$(BIN)/simplexdlclient: SimpleXDLClient.o  IPV4Socket.hh XDLRequest.hh
	$(CXX) $< -o $@ $(LIBGRAIL) $(LIBS)

$(BIN)/genericxdlclient: GenericXDLClient.cc IPV4Socket.hh XDLRequest.hh
	$(CXX) $< -o $@ $(LIBGRAIL) $(LIBS)

clean:
	rm -rf $(out) $(BIN)/simplexdlserver $(BIN)/simplexdlclient $(BIN)/genericxdlclient

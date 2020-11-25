include ${GRAIL}/proj/settings.mk


PARSEOBJ	:= std.o Buffer.o

.PHONY: all clean
all: $(BIN)/testsymbols $(BIN)/writetest $(BIN)/readtest $(BIN)/readUser


#$(BIN)/xdl


message:
	@echo "######################################################"
	@echo "# PLEASE build grail first"
	# cd ../grail; make
	@echo "######################################################"

WRITEOBJ	:= writetest.o Buffer.o std.o
$(BIN)/writetest: $(WRITEOBJ)
	$(CXX) $(WRITEOBJ) -o $(BIN)/writetest


writetest.o: writetest.cc
	$(COMP) $<

PRINTOBJ	:= tryPrint.o 
$(BIN)/tryPrint: $(PRINTOBJ)
	$(CXX) $(PRINTOBJ) -o $(BIN)/tryPrint

tryPrint.o: tryPrint.cc
	$(COMP) $<

READOBJ	:= readtest.o Buffer.o ErrNames.o
$(BIN)/readtest: $(READOBJ)
	$(CXX) $(READOBJ) -o $(BIN)/readtest

ErrNames.o: $(grail)/ErrNames.cc
	$(COMP) $<


readtest.o: readtest.cc
	$(COMP) $<

READUOBJ:= readUser.o Buffer.o 
$(BIN)/readUser: $(READUOBJ)
	$(CXX) $(READUOBJ) -o $(BIN)/readUser


readUser.o: readUser.cc
	$(COMP) $<

$(BIN)/xdl: message xdl.l xdl.y $(PARSEOBJ)
	flex xdl.l
	bison -d xdl.y #2> /dev/null
	gcc -c lex.yy.c html.tab.c
	$(COMP) util.cc main.cc
	$(LD) main.o lex.yy.o html.tab.o util.o $(PARSEOBJ) -o $(BIN)/grailparser
	rm -rf lex.yy.c html.tab.c html.tab.h

Buffer.o: $(util)/Buffer.cc $(util)/Buffer.hh
	$(COMP) $<

std.o: std.cc std.hh
	$(COMP) $<

TESTSYM	:= xdltest.o XDLCompiler.o SymbolTable.o Buffer.o datatype.o std.o
$(BIN)/testsymbols: $(TESTSYM)
	$(CXX) $(TESTSYM)  -o $(BIN)/testsymbols

SymbolTable.hh: $(util)/datatype.hh $(grail)/Errors.hh $(util)/Ex.hh

xdltest.o:  xdltest.cc SymbolTable.hh $(util)/Buffer.hh
	$(COMP) $<

XDLCompiler.o:  XDLCompiler.cc SymbolTable.hh $(util)/Buffer.hh
	$(COMP) $<

SymbolTable.o:  SymbolTable.cc SymbolTable.hh $(util)/Buffer.hh
	$(COMP) $<

datatype.o: $(util)/datatype.cc
	$(COMP) $<

clean:
	rm -rf *.o lex.yy.c html.tab.c html.tab.h html.output

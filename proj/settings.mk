BASE	:=	${GRAIL}
BIN	:=	${GRAIL}/bin
LIBDIR	:=	${GRAIL}/libs
LIBGRAIL:= 	$(LIBDIR)/libgrail.a
LIBSPEC	:=	-L${LIBDIR}

SRC	:=	${GRAIL}/src

util	:=	$(SRC)/util
opengl	:=	$(SRC)/opengl
csp	:=	$(SRC)/csp
xdl	:=	$(SRC)/xdl
audio := $(SRC)/audio

test	:=  ${GRAIL}/test

build	:= ${GRAIL}/build

utilout 		:= $(build)/util
openglout		:= $(build)/opengl
cspout			:= $(build)/csp
xdlout			:= $(build)/xdl
audioout		:= $(build)/audio


SOURCES := $(wildcard $(opengl)/*.cc $(opengl)/*.hh)

DEBUG	:=	-g
SANE	:=	#-fsanitize=address
OPT	:=	#-O2 -march=native
CCFLAGS	:=	$(DEBUG) $(SANE) $(OPT)

INC	:=	-I$(BASE)/src -I$(BASE)/include
LIBSPEC	:=	-L${GRAIL}/libs -L/usr/local/lib/
# ----

CXX	:=	g++  $(VERS) $(CCFLAGS) $(INC)
CXXI	:=	g++  $(VERS) $(DEBUG) $(OPT) $(INC)  #CXX INSANE so we don't have to deal with -f sanitize while debugging other stuff
COMP	:=	$(CXX) -c

LIBS	:=	-lgrail -lfreetype -lglfw -lGL -lX11 -ldl -pthread 
LD	:=	g++ $(VERS) $(CCFLAGS) $(LIBSPEC)

# Note that any recipes in this file will cause all makefiles to stop working
ifeq ($(OS),Windows_NT)
	LIBS	:=	$(LIBSPEC) -lgrail -lOpenGL32 -lz -mwindows -lfreetype -lglfw3 -lWs2_32 -lmpv
	# @echo "WINDOWS DETECTED"
else
	#assuming if not windows, then linux. This will break for unsupported OS or OS that does not declare properly. This will break on OSX.
	LIBS	:=	$(LIBSPEC) -L /usr/lib/x86_64-linux-gnu/ -lgrail -lfreetype -lglfw -lGL -lmpv -pthread -ldl
	# @echo "LINUX DETECTED"
endif

LD	:=	g++ $(VERS) $(CCFLAGS) $(LIBSPEC)

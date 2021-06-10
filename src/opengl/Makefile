include ${GRAIL}/proj/settings.mk
out=$(build)/opengl

# First the main classes GLWin, GLWinFonts,Tab, Canvas that contain all GUI
# The Shader class that interfaces to OpenGL shaders
# Then Shape and Shape2D, the base classes for all 2D drawing
# After that, all the components used to draw
# Last, dynamic, high-level classes like Document and DocumentView that aren't
# fully done yet

SHARED	:= $(out)/Glad.o \
$(out)/GLWin.o \
$(out)/GLWinFonts.o \
$(utilout)/Config.o $(utilout)/Prefs.o	\
$(utilout)/HashMap.o                    \
$(utilout)/Buffer.o			\
$(utilout)/datatype.o			\
$(cspout)/Socket.o				\
$(cspout)/IPV4Socket.o			\
$(cspout)/XDLRequest.o			\
$(cspout)/Request.o			\
$(cspout)/SocketIO.o 		\
$(xdlout)/XDLCompiler.o			\
$(xdlout)/SymbolTable.o			\
$(xdlout)/std.o				\
$(out)/ErrNames.o			\
$(out)/Tab.o 				\
$(out)/Canvas.o 			\
$(out)/Shader.o $(out)/Shape2D.o 	\
$(out)/Shape.o 				\
$(out)/Colors.o 			\
$(out)/Line.o  $(out)/Rectangle.o $(out)/Sector.o \
$(out)/Image.o \
$(out)/MultiShape.o $(out)/StyledMultiShape2D.o $(out)/MultiShape2D.o \
$(out)/MultiText.o $(out)/MultiShape3D.o $(out)/TextureArray.o \
$(out)/Document.o $(out)/DocView.o \
$(out)/LineGraphWidget.o $(out)/BarChartWidget.o $(out)/CandlestickChartWidget.o $(out)/BoxChartWidget.o \
$(out)/ButtonWidget.o $(out)/Scale.o \
$(out)/AngledMultiText.o \
$(out)/ScrollbarWidget.o \
#$(out)/AxesWidget.o $(out)/AxisWidget.o \

# heatmap does not yet work, so commented out until Dov or someone else gets it going. But it's VERY COOL!
#$(out)/HeatMap.o

#generateErrors

all: init  $(out)/Glad.o $(LIBGRAIL)
include ${GRAIL}/proj/common.mk

$(out)/Glad.o:
	echo "*****"
	echo $(out)
	$(CXX) -g -std=c++11 -c -I $(GRAIL)/include $(GRAIL)/include/glad/glad.c -o $@

$(LIBGRAIL): $(SHARED)
	cd $(out) ; ar rs $(LIBGRAIL) $(SHARED)

$(out)/GLWin.o: GLWin.cc GLWin.hh
	$(COMP) $< -o $@

$(out)/GLWinFonts.o: GLWinFonts.cc GLWinFonts.hh Errcode.hh
	$(COMP) $< -o $@

$(out)/Tab.o: Tab.cc Tab.hh Canvas.hh $(util)/DynArray.hh
	$(COMP) $< -o $@

$(utilout)/HashMap.o: $(util)/HashMap.cc $(util)/HashMap.hh
	$(COMP) $< -o $@

$(utilout)/Buffer.o: $(util)/Buffer.cc $(util)/Buffer.hh
	$(COMP) $< -o $@

$(utilout)/datatype.o: $(util)/datatype.cc $(util)/datatype.hh
	$(COMP) $< -o $@

$(cspout)/Socket.o: $(csp)/Socket.cc $(csp)/Socket.hh $(util)/Buffer.hh
	$(COMP) $< -o $@

$(cspout)/IPV4Socket.o: $(csp)/IPV4Socket.cc $(util)/Buffer.hh
	$(COMP) $< -o $@

$(cspout)/SocketIO.o: $(csp)/SocketIO.cc $(csp)/SocketIO.hh
	$(COMP) $< -o $@

$(xdlout)/SymbolTable.o: $(xdl)/SymbolTable.cc $(xdl)/SymbolTable.hh
	$(COMP) $< -o $@

$(cspout)/XDLRequest.o: $(csp)/XDLRequest.cc $(csp)/XDLRequest.hh
	$(COMP) $< -o $@

$(xdlout)/XDLCompiler.o: $(xdl)/XDLCompiler.cc $(xdl)/XDLCompiler.hh
	$(COMP) $< -o $@

$(cspout)/Request.o: $(csp)/Request.cc $(csp)/Request.hh
	$(COMP) $< -o $@

$(xdlout)/std.o: $(xdl)/std.cc $(xdl)/std.hh
	$(COMP) $< -o $@

$(out)/ErrNames.o: ErrNames.cc Errcode.hh
	$(COMP) $< -o $@

$(utilout)/Config.o: $(util)/Config.cc $(util)/Config.hh
	$(COMP) $< -o $@

$(utilout)/Prefs.o: $(util)/Prefs.cc $(util)/Prefs.hh
	$(COMP) $< -o $@

$(out)/Shader.o: Shader.cc Shader.hh
	$(COMP) $< -o $@

$(out)/Canvas.o: Canvas.cc Canvas.hh MultiShape2D.hh
	$(COMP) $< -o $@

$(out)/Colors.o: Colors.cc Colors.hh
	$(COMP) $< -o $@

$(out)/Shape.o: Shape.cc Shape.hh
	$(COMP) $< -o $@

$(out)/Image.o: Image.cc Image.hh
	$(COMP) $< -o $@

$(out)/Sector.o: Sector.cc Sector.hh
	$(COMP) $< -o $@

$(out)/Rectangle.o: Rectangle.cc Rectangle.hh
	$(COMP) $< -o $@



### Multishapes:--------------------------------------------------------------
$(out)/MultiShape.o: MultiShape.cc MultiShape.hh                            
	$(COMP) $< -o $@                           

$(out)/MultiShape2D.o: MultiShape2D.cc MultiShape2D.hh 
	$(COMP) $< -o $@

$(out)/StyledMultiShape2D.o: StyledMultiShape2D.cc StyledMultiShape2D.hh 
	$(COMP) $< -o $@

$(out)/MultiShape3D.o: MultiShape3D.cc MultiShape3D.hh 
	$(COMP) $< -o $@

$(out)/MultiText.o: MultiText.cc MultiText.hh
	$(COMP) $< -o $@

$(out)/AngledMultiText.o: AngledMultiText.cc AngledMultiText.hh
	$(COMP) $< -o $@

$(out)/Document.o: Document.cc Document.hh
	$(COMP) $< -o $@

$(out)/DocView.o: DocView.cc DocView.hh
	$(COMP) $< -o $@

$(out)/LineGraphWidget.o: LineGraphWidget.cc LineGraphWidget.hh
	$(COMP) $< -o $@

$(out)/BarChartWidget.o: BarChartWidget.cc BarChartWidget.hh
	$(COMP) $< -o $@

$(out)/CandlestickChartWidget.o: CandlestickChartWidget.cc CandlestickChartWidget.hh
	$(COMP) $< -o $@

$(out)/BoxChartWidget.o: BoxChartWidget.cc BoxChartWidget.hh
	$(COMP) $< -o $@

$(out)/ButtonWidget.o: ButtonWidget.cc ButtonWidget.hh
	$(COMP) $< -o $@

$(out)/Scale.o: Scale.cc Scale.hh
	$(COMP) $< -o $@

$(out)/ScrollbarWidget.o: ScrollbarWidget.cc ScrollbarWidget.hh
	$(COMP) $< -o $@

# $(out)/AxesWidget.o: AxesWidget.cc AxesWidget.hh
# 	$(COMP) $< -o $@

# $(out)/AxisWidget.o: AxisWidget.cc AxisWidget.hh
# 	$(COMP) $< -o $@


###--------------------------------------------------------------------------
$(out)/HeatMap.o: HeatMap.cc HeatMap.hh 
	$(COMP) $< -o $@

$(out)/TextureArray.o: util/TextureArray.cc util/TextureArray.hh 
	$(COMP) $< -o $@

$(out)/Line.o: Line.cc Line.hh
	$(COMP) $< -o $@

$(out)/Shape2D.o: Shape2D.cc Shape2D.hh
	$(COMP) $< -o $@


#$(out)/WebCursor.o: WebCursor.cc WebCursor.hh
#	$(COMP) $< -o $@

clean:
	rm -rf $(out)

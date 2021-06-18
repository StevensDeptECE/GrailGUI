include ${GRAIL}/proj/settings.mk
init: makeBuildDirectories

makeBuildDirectories:
	mkdir -p $(utilout) $(openglout) $(cspout) $(xdlout) $(libshapeout) $(xpout) $(LIBDIR) 

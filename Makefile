#
# main makefile for grail, partially a tutorial
# in order to build grail, first set the following environment variables in your .profile
#
# export GRAIL=~/git/proj/Grail  # this is my path, add your own
# export PATH=$GRAIL/bin:$PATH
#
# then run make in this top-level directory
#
include ${GRAIL}/proj/settings.mk

grail:
	@echo $(grail)
	cd $(opengl) && make
	cd $(csp) && make
	cd $(test) && make     #  && grailparser test.grail   this should generate out.cml (binary html)

quickClean:
	rm -rf ${GRAIL}/build ${GRAIL}/libs -f
	@# -f for remove files even if they dont exist ... ie don't throw an error because it's already clean. 

clean: quickClean
	cd $(util) && make clean ; cd $(csp) && make clean; cd $(opengl) && make clean ; cd ${GRAIL}/test && make clean

# after this, any demos should work
# example testDrawing
# example testGraph


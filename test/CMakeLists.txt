
# add_grail_executable([BINNAME <binary name>] SRC <src path> LIBS <libs>)

# 2D Graphics
add_subdirectory(2d)

# 3D Graphics
add_subdirectory(3d)

# CAD
add_subdirectory(CAD)

# Graphs
add_subdirectory(graph)

# GUI
add_subdirectory(gui)

# Maps
add_subdirectory(maps)


# Media
add_subdirectory(media)

# Networking

# Solar System
add_subdirectory(solarsystem)

# Util

# XDL
add_subdirectory(xdl)

# XP (Experimental)
add_subdirectory(xp)

message(STATUS "Finding all targets, this may take a while")
get_all_targets(GRAIL_TEST_TARGETS ".")
get_property(GRAIL_TEST_DIRS DIRECTORY "." PROPERTY SUBDIRECTORIES)

message(STATUS "Shortening all targets, this may take a while")

foreach(i ${GRAIL_TEST_DIRS})
	file(RELATIVE_PATH
		ITEM_PATH_REL # Output variable
		${CMAKE_CURRENT_SOURCE_DIR} # Base directory
		${i} # Absolute path to the file
	)
	list(APPEND GRAIL_TEST_DIRS_REL ${ITEM_PATH_REL})
endforeach(i ${GRAIL_TEST_DIRS})

message(STATUS "Caching all targets, this may take a while")
set(GRAIL_TEST_TARGETS ${GRAIL_TEST_TARGETS} CACHE STRING "List of valid test targets" FORCE)
set(GRAIL_TEST_DIRS_REL ${GRAIL_TEST_DIRS_REL} CACHE STRING "List of test subdirectories relative to test/" FORCE)

# Unsorted 
# add_grail_executable(SRC ../src/xp/decompressBlockLoader.cc LIBS grail lzma)
# add_grail_executable(SRC StockDemo.cc LIBS grail)







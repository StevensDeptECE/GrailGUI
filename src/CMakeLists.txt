add_subdirectory(csp)
add_subdirectory(data)
add_subdirectory(opengl)
add_subdirectory(util)
add_subdirectory(xdl)
add_subdirectory(CAD)

set(grail-path
            ${PROJECT_SOURCE_DIR}/include/glad/glad.c
            ${grail-csp}
            ${grail-data}
            ${grail-opengl}
            ${grail-util}
            ${grail-xdl}
	          ${grail-CAD}
)

if(${GRAIL_EXPERIMENTAL})
  add_subdirectory(xp)
  list(APPEND grail-path ${grail-xp})
endif()

if(${MPV_FOUND} AND ${PC_MPV_VERSION} VERSION_GREATER "1.26")
  add_subdirectory(audio)
  add_subdirectory(video)
  list(APPEND grail-path ${grail-audio} ${grail-video})
endif()

# add_library(grailserver ${grail-path})

add_library(grail ${grail-path} opengl/Main.cc)

# GLFW
target_include_directories(grail INTERFACE ${GLFW_INCLUDE_DIRS})
target_link_libraries(grail glfw)

# target_include_directories(grailserver INTERFACE ${GLFW_INCLUDE_DIRS})
# target_link_libraries(grailserver glfw)

# GLM
target_include_directories(grail INTERFACE ${GLM_INCLUDE_DIRS})

# target_include_directories(grailserver INTERFACE ${GLM_INCLUDE_DIRS})

# opengl
target_include_directories(grail PRIVATE ${OPENGL_INCLUDE_DIRS})

# target_include_directories(grailserver PRIVATE ${OPENGL_INCLUDE_DIRS})

# Freetype
target_link_libraries(grail ${FREETYPE_LIBRARIES})
target_include_directories(grail PRIVATE ${FREETYPE_INCLUDE_DIRS})

# target_link_libraries(grailserver ${FREETYPE_LIBRARIES})
# target_include_directories(grailserver PRIVATE ${FREETYPE_INCLUDE_DIRS})

# DL
target_link_libraries(grail ${CMAKE_DL_LIBS})

# target_link_libraries(grailserver ${CMAKE_DL_LIBS})

# MPV
if(${MPV_FOUND} AND ${PC_MPV_VERSION} VERSION_GREATER "1.26")
  target_link_libraries(grail ${MPV_LIBRARY})
  target_include_directories(grail INTERFACE ${MPV_INCLUDE_DIR})

  # target_link_libraries(grailserver ${MPV_LIBRARY})
  # target_include_directories(grailserver INTERFACE ${MPV_INCLUDE_DIR})
endif()

# fmtlib
target_link_libraries(grail fmt::fmt)

# target_link_libraries(grailserver fmt::fmt)

# Shapelib
target_link_libraries(grail shpgrail)

#Webp
target_link_libraries(grail webp)

# target_link_libraries(grailserver shpgrail)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries(grail wsock32 ws2_32)
	
  # target_link_libraries(grailserver wsock32 ws2_32)
endif()

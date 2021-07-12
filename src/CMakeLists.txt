add_subdirectory(csp)
add_subdirectory(data)
add_subdirectory(opengl)
add_subdirectory(util)
add_subdirectory(xdl)

set(grail-path
            ${PROJECT_SOURCE_DIR}/include/glad/glad.c
            ${grail-csp} 
            ${grail-data}
            ${grail-opengl}
            ${grail-util}
            ${grail-xdl}
            )


if(${GRAIL_EXPERIMENTAL})
  add_subdirectory(xp)
  list(APPEND grail-path ${grail-xp})
endif()

if(${MPV_FOUND})
  add_subdirectory(audio)
  add_subdirectory(video)
  list(APPEND grail-path ${grail-audio} ${grail-video})
endif()

add_library(grail ${grail-path})

# GLFW
target_include_directories(grail INTERFACE ${GLFW_INCLUDE_DIRS})
target_link_libraries(grail glfw)

# GLM
target_include_directories(grail INTERFACE ${GLM_INCLUDE_DIRS})

# opengl
target_include_directories(grail PRIVATE ${OPENGL_INCLUDE_DIRS})

# Freetype
target_link_libraries(grail ${FREETYPE_LIBRARIES})
target_include_directories(grail PRIVATE ${FREETYPE_INCLUDE_DIRS})

# DL
target_link_libraries(grail ${CMAKE_DL_LIBS})

# MPV
if(${MPV_FOUND})
  target_link_libraries(grail ${MPV_LIBRARY})
  target_include_directories(grail INTERFACE ${MPV_INCLUDE_DIR})
endif()

# Shapelib
target_link_libraries(grail shpgrail)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries(grail wsock32 ws2_32)
endif()

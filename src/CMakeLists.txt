add_subdirectory(csp)
add_subdirectory(data)
add_subdirectory(opengl)
add_subdirectory(util)
add_subdirectory(xdl)

add_library(grail
						${grail-csp} 
						${grail-data}
						${grail-opengl}
						${grail-util}
						${grial-xdl})


if(${GRAIL_EXPERIMENTAL})
add_subdirectory(xp)
endif()

# GLFW
target_include_directories(grail INTERFACE ${GLFW_INCLUDE_DIRS})
target_link_libraries(grail PRIVATE ${GLFW_LIBRARY_DIRS})

# opengl
target_include_directories(grail PRIVATE ${OPENGL_INCLUDE_DIRS})

# Freetype
target_link_libraries(grail INTERFACE ${FREETYPE_LIBRARIES})
target_include_directories(grail PRIVATE ${FREETYPE_INCLUDE_DIRS})
add_subdirectory(audio)
add_subdirectory(csp)
add_subdirectory(data)
add_subdirectory(opengl)
add_subdirectory(util)
add_subdirectory(xdl)

add_library(grail
            ${PROJECT_SOURCE_DIR}/include/glad/glad.c
            ${grail-audio}
            ${grail-csp} 
            ${grail-data}
            ${grail-opengl}
            ${grail-util}
            ${grail-xdl})

if(${GRAIL_EXPERIMENTAL})
add_subdirectory(xp)
endif()

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
target_link_libraries(grail PkgConfig::MPV)

# Shapelib
target_link_libraries(grail shpgrail)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
	target_link_libraries(grail wsock32 ws2_32)
endif()

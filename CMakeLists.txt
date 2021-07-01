cmake_minimum_required(VERSION 3.16.1)

project(Grail VERSION 0.1.0
DESCRIPTION
"Grail"
LANGUAGES
C CXX)


set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)
set(CMAKE_FILES_DIRECTORY build) 
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)
set(CMAKE_CXX_STANDARD 20)
option(CMAKE_CXX_STANDARD_REQUIRED ON)
option(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
option(CMAKE_LINK_WHAT_YOU_USE ON)
set(GRAIL_EXPERIMENTAL "OFF")
set(CMAKE_DEBUG_POSTFIX d)

string(TOLOWER "${CMAKE_BUILD_TYPE}" build_type_lower)

if(build_type_lower EQUAL "debug")
    message(STATUS "Debug mode:")
    message(STATUS "Building shared libs")
    option(BUILD_SHARED_LIBS ON)
else()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT supported OUTPUT error)

    if( supported )
        message(STATUS "IPO / LTO enabled")
        set(TARGET grail PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    else()
        message(STATUS "IPO / LTO not supported: <${error}>")
    endif()
endif()


include(FetchContent)
include(GrailFunctions)

find_package(PkgConfig REQUIRED) 

# opengl
find_package(OpenGL REQUIRED)

# GLFW
find_package(glfw3 REQUIRED)

# Freetype
find_package(Freetype REQUIRED)

# MPV
pkg_check_modules(MPV REQUIRED IMPORTED_TARGET mpv)

# GLM
FetchContent_Declare(
    glm  
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    # 0.9.9.8 does not have C++ 20 fix yet, so master branch is currently used
    #GIT_TAG        bf71a834948186f4097caa076cd2663c69a10e1e # 0.9.9.8
    SOURCE_DIR     ${PROJECT_SOURCE_DIR}/external/glm
)
FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

# Shapelib
# Currently manually compiled due to compiler warnings
## Output is not shown, so errors may occur without notice
##option(FETCHCONTENT_QUIET "Enables QUIET option for all content population" ON)
##FetchContent_Declare(
##    shapelib
##    GIT_REPOSITORY https://github.com/OSGeo/shapelib.git
##    #GIT_TAG       The tags have not been updated since 2019, so no tag
##    SOURCE_DIR     ${PROJECT_SOURCE_DIR}/external/shapelib
##)
##FetchContent_MakeAvailable(shapelib)

add_subdirectory(include/libshape)
add_subdirectory(src)

target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/external/glm)
target_include_directories(grail PUBLIC ${PROJECT_SOURCE_DIR}/external)


add_subdirectory(test)

#dump_cmake_variables()

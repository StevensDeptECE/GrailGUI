function(dump_cmake_variables)
    get_cmake_property(_variableNames VARIABLES)
    list(SORT _variableNames)

    foreach(_variableName ${_variableNames})
        if(ARGV0)
            unset(MATCHED)
            string(REGEX MATCH ${ARGV0} MATCHED ${_variableName})

            if(NOT MATCHED)
                continue()
            endif()
        endif()

        message(STATUS "${_variableName}=${${_variableName}}")
    endforeach()
endfunction()

function(add_grail_executable)
    set(oneValueArgs SRC BINNAME)
    set(options)
    set(multiValueArgs LIBS)
    cmake_parse_arguments(GRAILBIN "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})

    if(NOT GRAILBIN_BINNAME)
        get_filename_component(GRAILBIN_BINNAME ${GRAILBIN_SRC} NAME_WE)
    endif()

    add_executable(${GRAILBIN_BINNAME} ${GRAILBIN_SRC})
    target_link_libraries(${GRAILBIN_BINNAME} ${GRAILBIN_LIBS})
endfunction()

function(load_python)
    set(oneValueArgs SRC DICT)
    set(options)
    set(multiValueArgs)
    cmake_parse_arguments(GRAILPY "${options}" "${oneValueArgs}"
        "${multiValueArgs}" ${ARGN})
    set(scriptPath ${GRAILPY_SRC})
    set(filePath ${GRAILPY_DICT})
    string(PREPEND filePath ${CMAKE_CURRENT_SOURCE_DIR}/)
    string(PREPEND scriptPath ${CMAKE_CURRENT_SOURCE_DIR}/)
    get_filename_component(fileName ${filePath} NAME_WE)
    set(hashType SHA256)
    file(${hashType} ${filePath} fileHash)

    if(NOT "${fileHash}" STREQUAL "${GRAIL_${fileName}}")
        if(GRAIL_${fileName})
            message(STATUS "${fileName} updated")
            message(STATUS "[old ${hashType}]: ${GRAIL_${fileName}}")
            message(STATUS "[new ${hashType}]: ${fileHash}")
        endif()

        set(GRAIL_${fileName} ${fileHash} CACHE INTERNAL "SHA512 hash of file ${fileName}")
        execute_process(COMMAND ${scriptPath} WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
    endif()
endfunction()

# Collect all currently added targets in all subdirectories
#
# Parameters:
# - _result the list containing all found targets
# - _dir root directory to start looking from
function(get_all_targets _result _dir)
    get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)

    foreach(_subdir IN LISTS _subdirs)
        get_all_targets(${_result} "${_subdir}")
    endforeach()

    get_directory_property(_sub_targets DIRECTORY "${_dir}" BUILDSYSTEM_TARGETS)
    set(${_result} ${${_result}} ${_sub_targets} PARENT_SCOPE)
endfunction()

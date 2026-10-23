cmake_minimum_required(VERSION 3.50)
project(WorldBuilder VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(GIT_SUBMODULE "allow git submodule auto update" ON)

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    message("==== Create .clang file to store 'CompilationDatabase: build/' ====")
    file(WRITE ".clang"
        "CompileFlags:
            CompilationDatabase: build/"
    )
endif()

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND "${GIT_EXECUTABLE} submodule update --init --recursive --checkout"
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        COMMAND_ECHO STDOUT)
    endif()
endif()

set(BIN_OUTPUT_DIR "/bin/${CMAKE_SYSTEM_NAME}")
set(OUTPUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}${BIN_OUTPUT_DIR}")

include(RandomGraphsParkour/RandomGraphsParkour.cmake)
include(RandomGraphsParkour/vendors/GLFW/GLFW.cmake)
include(RandomGraphsParkour/vendors/glad/glad.cmake)

cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../dll)
set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0")

project(
        Project.Nar.Base
        VERSION 100
        DESCRIPTION "Nar.Base"
        LANGUAGES C
)

add_library(Nar.Base SHARED
        _package.c
        _package.h
        array.c
        basics.c
        bitwise.c
        char.c
        debug.c
        list.c
        math.c
        string.c
        utils.c
)

target_include_directories(Nar.Base PRIVATE ../../../nar-runtime-c/include)
set_target_properties(Nar.Base PROPERTIES VERSION ${PROJECT_VERSION})

target_link_libraries(Nar.Base)

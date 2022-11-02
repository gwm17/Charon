cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_BUILD_TYPE "Debug")
    message("Building debug")
else()
    message("Building release")
endif()

project(Charon)

set(CHARON_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
#assume we've installed nscldaq to the standard location
set(NSCLDAQ_DIR /usr/opt/opt-buster/)

add_subdirectory(src)
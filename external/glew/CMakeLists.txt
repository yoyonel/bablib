cmake_minimum_required(VERSION 2.8)
project(GLEW)

INCLUDE(/usr/share/cmake-2.8/Modules/ExternalProject.cmake)

MESSAGE(${CMAKE_CURRENT_BINARY_DIR}/external/glew)
ExternalProject_Add(GLEW
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/glew
	SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/external/glew
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND qmake-qt4 ${CMAKE_CURRENT_BINARY_DIR}/external/glew/glew.pro PREFIX=${CMAKE_CURRENT_BINARY_DIR}/external/glew
    BUILD_COMMAND make
 )

cmake_minimum_required (VERSION 2.6)

project (Bablib)

file( GLOB BABLIB_SRC src/*.cpp )

###############
# DEPENDANCES #
###############

#############
# QGLViewer #
#############
SET(QGLViewer_GIT_REPO https://github.com/yoyonel/libqglviewer.git)

INCLUDE(/usr/share/cmake-2.8/Modules/ExternalProject.cmake)
ExternalProject_Add(QGLViewer
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/external/qglviewer
    GIT_REPOSITORY ${QGLViewer_GIT_REPO}
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND qmake-qt4 QGLViewer/QGLViewer.pro QGLVIEWER_STATIC=yes PREFIX=${CMAKE_CURRENT_BINARY_DIR}/external/qglviewer
    BUILD_COMMAND make
 )
#############

find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})



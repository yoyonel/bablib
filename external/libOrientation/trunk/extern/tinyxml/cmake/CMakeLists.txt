# TinyXML Root CMakeLists.txt
# author: Bruno VALLET
# date: 23/10/2009
# Manages TinyXML project

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# PROJECT NAME AND TYPE.
PROJECT(TinyXML CXX)

SET(PACKAGE "TinyXML")
SET(PACKAGE_NAME "Tiny XML")
SET(PACKAGE_STRING ".xml handling library")
SET(PACKAGE_TARNAME "TinyXML")
SET(PACKAGE_VERSION "1.0")
SET(VERSION "1.0")

include(../../cmake/MatisMacros.cmake)

__matis_extern_project()

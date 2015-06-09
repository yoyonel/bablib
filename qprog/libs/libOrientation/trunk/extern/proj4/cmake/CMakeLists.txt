# proj4 Root CMakeLists.txt
# authors: Nicolas DAVID and Bruno VALLET
# date: 23/10/2009
# Manages proj4 project

cmake_minimum_required(VERSION 2.6)

# PROJECT NAME AND TYPE.
project(proj4 C)

set(PACKAGE "proj4")
set(PACKAGE_BUGREPORT "warmerdam@pobox.com")
set(PACKAGE_NAME "PROJ.4 Projections")
set(PACKAGE_STRING "PROJ.4 Projections 4.6.0")
set(PACKAGE_TARNAME "proj4")
set(PACKAGE_VERSION "4.6.0")
set(VERSION "4.6.0")

include(../../cmake/MatisMacros.cmake)

if(CUSTOM_BUILD)
	option(CONVERT_DATA "convert some ascii file to binary file for use in proj4" OFF)
	option(INSTALL_DATA "Install files /data /lib /exe html" OFF)
else(CUSTOM_BUILD)
	set(CONVERT_DATA OFF)
	set(INSTALL_DATA OFF)
endif(CUSTOM_BUILD)

__matis_extern_project()

if(INSTALL_DATA OR CONVERT_DATA)
	include(${PROJECT_SOURCE_DIR}/proj4-data.cmake OPTIONAL RESULT_VARIABLE proj4_DATA_EXISTS)
	if(NOT proj4_DATA_EXISTS)
		message(STATUS "ATTENTION: No ${PROJECT_SOURCE_DIR}/proj4-data.cmake found, data will not be installed nor converted")
	endif(NOT proj4_DATA_EXISTS)
endif(INSTALL_DATA OR CONVERT_DATA)


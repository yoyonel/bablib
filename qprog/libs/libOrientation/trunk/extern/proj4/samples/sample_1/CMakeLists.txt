CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# PROJECT NAME AND TYPE.
PROJECT(PROJ4_sample1 CXX C)

# SET classical path for the MATIS library
#
IF(UNIX)
	SET(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH}  )
ENDIF(UNIX)
IF(WIN32)
	SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files/MATIS" )
ENDIF(WIN32)


#
#
add_executable(sample1 sample1.cpp)

find_package(proj4 QUIET)
IF(proj4_FOUND)
	SET(PROJ4_INSTALL_DIR "proj4")
	include_directories(${proj4_INCLUDE_DIRS})
ELSE(proj4_FOUND)
	SET(ROOT_DIR ${PROJECT_SOURCE_DIR})
	SET(PROJ4_INSTALL_DIR "proj4")
	SET(CUSTOM_BUILD ON)
	SET(PROJ4_ROOT_DIR ${ROOT_DIR}/../..)
	include( ${ROOT_DIR}/../../cmake/libproj4/CMakeLists.txt )
ENDIF(proj4_FOUND)

target_link_libraries(sample1 proj4)

# - Try to find lib3ds
# Once done this will define
#
#  Lib3DS_FOUND - system has 3ds
#  Lib3DS_INCLUDE_DIR - the 3ds include directory
#  Lib3DS_LIBRARIES - Link these to use 3ds
#
# Lib prefix is use in order to avoid using "3" as first letter of cmake variable because such
# variable have different behavior than classical CMake variable. ( Policy CMP0012)

FIND_PATH(Lib3DS_INCLUDE_DIR lib3ds/io.h
	PATHS
	/usr/include
 	/usr/local/include
	C:/Program Files/MATIS/lib3ds/include
)

FIND_LIBRARY(Lib3DS_LIBRARIES
	NAMES 3ds lib3ds
	PATHS
	/usr/lib
	/usr/local/lib
	C:/Program Files/MATIS/lib3ds/lib
)

IF(Lib3DS_INCLUDE_DIR AND Lib3DS_LIBRARIES)
	SET(Lib3DS_FOUND TRUE)
ENDIF(Lib3DS_INCLUDE_DIR AND Lib3DS_LIBRARIES)

IF(Lib3DS_FOUND)
	IF(NOT Lib3DS_FIND_QUIETLY)
		MESSAGE(STATUS "Found lib3ds: ${Lib3DS_LIBRARIES}")
	ENDIF(NOT Lib3DS_FIND_QUIETLY)
ELSE(Lib3DS_FOUND)
	IF(Lib3DS_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find lib3ds")
	ENDIF(Lib3DS_FIND_REQUIRED)
ENDIF(Lib3DS_FOUND)
# show the 3DS_INCLUDE_DIR and 3DS_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED(Lib3DS_INCLUDE_DIR Lib3DS_LIBRARIES)

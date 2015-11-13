# adds TinyXML library targets
# author: Bruno VALLET
# date: 23/10/2009

# define source and header files and directories
set(TinyXML_SRCS
tinystr.cpp
tinyxml.cpp
tinyxmlerror.cpp
tinyxmlparser.cpp)
__global_append(TinyXML_SRCS TinyXML_FULL_SRCS "${TinyXML_ROOT_DIR}/")

include_directories(${TinyXML_ROOT_DIR})
set(TinyXML_HEADERS
tinystr.h
tinyxml.h
)
__global_append(TinyXML_HEADERS TinyXML_FULL_HEADERS "${TinyXML_ROOT_DIR}/")

# set definitions and dependencies
set(TinyXML_DEFINITIONS "")
option(TIXML_USE_STL "Let TinyXML use the Standard Template Library" ON)
if(TIXML_USE_STL)
	set(TinyXML_DEFINITIONS ${TinyXML_DEFINITIONS} -DTIXML_USE_STL)
	set(TinyXML_TARGET_NAME TinyXML_STL)
else(TIXML_USE_STL)
	set(TinyXML_TARGET_NAME TinyXML)
endif(TIXML_USE_STL)
set(TinyXML_DEPENDENCIES "")

if(WIN32 AND BUILD_SHARED_LIBS)
	message("Warning: TinyXml cannot be built as a DLL")
	set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
endif(WIN32 AND BUILD_SHARED_LIBS)

# call the macro from Extern/cmake/CommonMacros.cmake that cleanfully adds a library
__add_library (${TinyXML_TARGET_NAME} TinyXML_FULL_SRCS TinyXML_FULL_HEADERS TinyXML_DEFINITIONS TinyXML_DEPENDENCIES)

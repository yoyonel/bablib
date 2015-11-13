OPTION(CUSTOM_BUILD "if not set build all targets on SGL project" OFF)

#include doxygen macro
include(extern/cmake/MatisDoxygenMacros.cmake)
set(CMAKE_VERBOSITY 1)

#config documentation
__config_documentation()

#set up src using by doxygen documentation
set(LIBORI_HPP_DIR ${SRC_DIR})
SET(DOXY_EXTRACT_ALL "YES")
SET(DOXY_COLS_IN_ALPHA_INDEX "4")
	
file(GLOB LIBORI_DOC_SRC ${LIBORI_HPP_DIR}*.hpp)
__add_documentation(libORI LIBORI_DOC_SRC)

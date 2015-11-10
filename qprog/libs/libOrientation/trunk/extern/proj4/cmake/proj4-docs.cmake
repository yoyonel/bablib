# adds PROJ4 documentation
# authors: Nicolas DAVID and Bruno VALLET
# date: 23/10/2009

set(DOCS_DIR ${CMAKE_SOURCE_DIR}/../docs)
set(SRC_DIR ${PROJ4_ROOT_DIR}/src)
						 
#include doxygen macro
include(../../cmake/MatisDoxygenMacros.cmake)

#config documentation
__config_documentation()

#set up src using by doxygen documentation
SET( PROJ4_DOC_SRC ${SRC_DIR}/projects.h 
				${DOCS_DIR}/doxygen/doxygen.dox 
				${DOCS_DIR}/doxygen/proj_api_module.dox 
				${DOCS_DIR}/doxygen/proj_impl_module.dox 
				${DOCS_DIR}/doxygen/proj_api.dox 
				${SRC_DIR}/pj_transform.c 
				${SRC_DIR}/proj_api.h )
set(DOXYGEN_ALPHA_PREFIX "" )
__add_documentation(PROJ4 PROJ4_DOC_SRC)

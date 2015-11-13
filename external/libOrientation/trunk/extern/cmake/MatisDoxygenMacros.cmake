# adds  documentation targets
# authors: Nicolas DAVID
# date: 20/11/2009


macro(__config_documentation)
	set(DOCUMENTATION_ENABLED ON)
	if(CMAKE_VERBOSITY GREATER 0)
			message(STATUS "--------- configure documentation ---------" )
	endif(CMAKE_VERBOSITY GREATER 0)
	
	find_package(Doxygen)
	if(DOXYGEN_EXECUTABLE)	
		if(CMAKE_VERBOSITY GREATER 0)
			message(STATUS " found doxygen ")
		endif(CMAKE_VERBOSITY GREATER 0)
	else(DOXYGEN_EXECUTABLE)
		if(CMAKE_VERBOSITY GREATER 0)
			message(STATUS " doxygen not found")
		endif(CMAKE_VERBOSITY GREATER 0)
		set(DOCUMENTATION_ENABLED OFF)
	endif(DOXYGEN_EXECUTABLE)
	
	if(NOT MATIS_DOC_DIR)
		if(UNIX)
			set(MATIS_DOC_DIR "/usr/local/doc" CACHE PATH " install path" )
		endif(UNIX)
		if(WIN32)
			set(MATIS_DOC_DIR "C:/Program Files/MATIS/doc" CACHE PATH " install path")
		endif(WIN32)
	endif(NOT MATIS_DOC_DIR)
	
	if(NOT MATIS_DOC_COMMON_DIR)
		SET(MATIS_DOC_COMMON_DIR ${MATIS_DOC_DIR}/matis_doc_template )
		SET(MATIS_DOC_TRAC_DIR ${MATIS_DOC_DIR}/trac )
	endif(NOT MATIS_DOC_COMMON_DIR)
	
	find_path( MATIS_DOXYFILE_DIR  
				NAMES 
					MatisDoxyfile.cmake.in
				PATHS 
					${MATIS_DOC_COMMON_DIR}/doxygen 
					${CMAKE_SOURCE_DIR}/../../cmake/doxygen
			 )
	if(NOT MATIS_DOXYFILE_DIR)
		if(CMAKE_VERBOSITY GREATER 0)
			message(STATUS " MatisDoxyfile.cmake.in NOT found in " ${MATIS_DOC_COMMON_DIR} " " ${CMAKE_SOURCE_DIR}/../../cmake/doxygen  )
			message(STATUS " set a valid path for MATIS_DOC_COMMON_DIR " )
		endif(CMAKE_VERBOSITY GREATER 0)
		set(DOCUMENTATION_ENABLED OFF)
		set(DOXYFILE_FOUND OFF)
	else(NOT MATIS_DOXYFILE_DIR)
		if(CMAKE_VERBOSITY GREATER 0)
			message(STATUS " MatisDoxyfile.cmake.in found  in " ${MATIS_DOXYFILE_DIR} )
		endif(CMAKE_VERBOSITY GREATER 0)
		set(MATIS_DOXYFILE ${MATIS_DOXYFILE_DIR}/MatisDoxyfile.cmake.in)
		set(DOXYFILE_FOUND ON)
	endif(NOT MATIS_DOXYFILE_DIR)
	
	if(NOT TARGET doc)
		add_custom_target(doc)
	endif(NOT TARGET doc)
		
	SET(DOXY_EXTRACT_ALL "NO")	
	SET(DOXY_COLS_IN_ALPHA_INDEX "5")	
		
	if(CMAKE_VERBOSITY GREATER 0)
			message(STATUS " Documentation enabled "  ${DOCUMENTATION_ENABLED} )
			message(STATUS "--------- end configure documentation ---------" )
	endif(CMAKE_VERBOSITY GREATER 0)
	
endmacro(__config_documentation)


macro(__install_shared_documentation)
	set(INSTALL_SHARED_DOCUMENTATION OFF)
	set( SHARED_DOC_EXIST OFF)
	
	# test if shared doc is yet installed
	string(COMPARE EQUAL ${MATIS_DOXYFILE_DIR} ${MATIS_DOC_COMMON_DIR}  SHARED_DOC_EXIST)
	if(CMAKE_VERBOSITY GREATER 1)
		if(SHARED_DOC_EXIST)
			message(STATUS " shared doc yet install in "  ${MATIS_DOC_COMMON_DIR} )
		else(SHARED_DOC_EXIST)
			message(STATUS "install shared doc from "  ${MATIS_DOXYFILE_DIR} " to "  ${MATIS_DOC_COMMON_DIR})
		endif(SHARED_DOC_EXIST)
	endif(CMAKE_VERBOSITY GREATER 1)
	
	if(DOXYFILE_FOUND AND NOT SHARED_DOC_EXIST AND INSTALL_ALL_DOC )
		set(INSTALL_SHARED_DOCUMENTATION ON)
	endif(DOXYFILE_FOUND AND NOT SHARED_DOC_EXIST AND INSTALL_ALL_DOC )

	# install target if needed
	if(INSTALL_SHARED_DOCUMENTATION)
		install (DIRECTORY ${MATIS_DOXYFILE_DIR} DESTINATION ${MATIS_DOC_COMMON_DIR} REGEX "/.svn*" EXCLUDE)
	endif(INSTALL_SHARED_DOCUMENTATION)
	
	file(GLOB CSS_FILE ${MATIS_DOXYFILE_DIR}/*.css )
	file(GLOB PNG_FILE ${MATIS_DOXYFILE_DIR}/*.png )
	file(GLOB GIF_FILE ${MATIS_DOXYFILE_DIR}/*.gif )
	file(GLOB SEARCH_FILE ${MATIS_DOXYFILE_DIR}/search/*.png ${MATIS_DOXYFILE_DIR}/search/*.css ${MATIS_DOXYFILE_DIR}/search/*.js )
	if(CMAKE_VERBOSITY GREATER 1)
		message(STATUS "-Search for file " ${MATIS_DOXYFILE_DIR}/search/*.[png-css-js])
		message(STATUS " results in : " ${SEARCH_FILE})
	endif(CMAKE_VERBOSITY GREATER 1)
	foreach( doc_path ${INSTALL_PATH})
		install ( FILES ${DOXYGEN_HTML_CSS} DESTINATION ${doc_path} )
		install (FILES ${CSS_FILE} DESTINATION ${doc_path} )
		install (FILES ${PNG_FILE} DESTINATION ${doc_path} )
		install (FILES ${GIF_FILE} DESTINATION ${doc_path} )
		install (FILES ${SEARCH_FILE} DESTINATION ${doc_path}/search )
	endforeach( doc_path ${INSTALL_PATH})
	
	
endmacro(__install_shared_documentation)


#variables that may be defined to use macro :
# DOC_DESCRIPTION
# DOC_VERSION
# DOC_AUTOBRIEF
# DOC_PREPROCESSING
##################################
### ADD A DOCUMENTATION TARGET ###
##################################
macro(__add_documentation _exe_name _source )
if(CMAKE_VERBOSITY GREATER 1)
	MESSAGE(STATUS "---------  documentation ---------"  ${exe_name})
endif(CMAKE_VERBOSITY GREATER 1)
 if(DOCUMENTATION_ENABLED)
 	if(CMAKE_VERBOSITY GREATER 1)
			MESSAGE(STATUS " documentation enabled, processing...")
	endif(CMAKE_VERBOSITY GREATER 1)
	#0 set some variables.
	set(${_exe_name}_DOCS_DIR ${CMAKE_BINARY_DIR}/doc)
	if(CMAKE_VERBOSITY GREATER 1)
			MESSAGE(STATUS " .doc output dir :  " ${${_exe_name}_DOCS_DIR})
	endif(CMAKE_VERBOSITY GREATER 1)
	set( ${MATIS_DOC_COMMON_DIR} )
	if(NOT DEFINED DOC_DESCRIPTION)
		set(DOC_DESCRIPTION "${_exe_name} library API" )
	endif(NOT DEFINED DOC_DESCRIPTION)
	if(NOT DEFINED DOC_VERSION)
		set(DOC_VERSION 0.0 )
	endif(NOT DEFINED DOC_VERSION)
	if(NOT DEFINED DOC_AUTOBRIEF)
		set(DOC_AUTOBRIEF "NO" )
	endif(NOT DEFINED DOC_AUTOBRIEF)
	if(NOT DEFINED DOC_PREPROCESSING)
		set(DOC_PREPROCESSING "YES" )
	endif(NOT DEFINED DOC_PREPROCESSING)
	
	if( NOT DOXYGEN_ALPHA_PREFIX)
		string(TOUPPER ${_exe_name} UPPER_PREFIX)
  		string(TOLOWER ${_exe_name} LOWER_PREFIX)
		set(DOXYGEN_ALPHA_PREFIX "${UPPER_PREFIX} ${LOWER_PREFIX}" )
	endif( NOT DOXYGEN_ALPHA_PREFIX)
	#1  set default options and use custom build to enable or not to change default configuration
	if(CUSTOM_BUILD)
		option( ENABLE_${_exe_name}_DOXYGEN_DOCS " build classical matis doxygen doc with doc target " ON )
		option( ENABLE_${_exe_name}_TRAC_DOXYGEN " build matis doxygen doc for trac site/doxygen plugin " OFF)
		option( INSTALL_ALL_DOC " install generate documentation " OFF)
		set( DOXYGEN_HTML_HEADER "Header.html" CACHE PATH " html header")
		set( DOXYGEN_HTML_FOOTER "Footer.html" CACHE PATH " html footer")
		set( DOXYGEN_HTML_CSS ${MATIS_DOXYFILE_DIR}/doxygen.css CACHE PATH " doxygen css file")
		set( DOXY_ALPHA_PREFIX ${DOXYGEN_ALPHA_PREFIX} CACHE PATH " doxygen ignore prefix for alphabetical sort")
	else(CUSTOM_BUILD)
		set(ENABLE_${_exe_name}_DOXYGEN_DOCS  ON)
		set(ENABLE_${_exe_name}_TRAC_DOXYGEN OFF)
		set( INSTALL_ALL_DOC OFF)
		set( DOXYGEN_HTML_HEADER "")
		set( DOXYGEN_HTML_FOOTER "")
		set( DOXYGEN_HTML_CSS "")
		set( DOXY_ALPHA_PREFIX ${DOXYGEN_ALPHA_PREFIX} )
	endif(CUSTOM_BUILD)
	set(INSTALL_PATH "")
	
	#2 process _source
	SET(DOC_SRC_INI ${${_source}})
	SET(DOC_SRC "")
	foreach(DOC_FILE ${DOC_SRC_INI})
		if(CMAKE_VERBOSITY GREATER 1)
			MESSAGE(STATUS " add source file " ${DOC_FILE} " to documentation" )
		endif(CMAKE_VERBOSITY GREATER 1)
  		 set(DOC_SRC "${DOC_SRC} ${DOC_FILE}")
 	endforeach(DOC_FILE ${DOC_SRC_INI})
	
	
	set( DOC_HTML_HEADER ${DOXYGEN_HTML_HEADER})
	set( DOC_HTML_FOOTER ${DOXYGEN_HTML_FOOTER})
	set( DOC_HTML_CSS ${DOXYGEN_HTML_CSS})

	#3 generate doxyfile with configure_file command from MatisDoxyfile.cmake.in
	if(ENABLE_${_exe_name}_DOXYGEN_DOCS )
		if(CMAKE_VERBOSITY GREATER 1)
			MESSAGE(STATUS " ... configure doxygen doc result in :  " ${${_exe_name}_DOCS_DIR}/Doxyfile)
		endif(CMAKE_VERBOSITY GREATER 1)
		set(DOC_OUTPUT ${${_exe_name}_DOCS_DIR})
		set( DOC_TYPE "html")
		set( DOC_OUTPUT_SUFFIX "html")
		configure_file(${MATIS_DOXYFILE} ${${_exe_name}_DOCS_DIR}/Doxyfile @ONLY)
	endif(ENABLE_${_exe_name}_DOXYGEN_DOCS)
	
	if(ENABLE_${_exe_name}_TRAC_DOXYGEN)
		set( DOC_TYPE "html")
		set( DOC_OUTPUT_SUFFIX "trac")
		set( DOC_HTML_HEADER "TracHeader.html")
		set( DOC_HTML_FOOTER "TracFooter.html")
		configure_file(${MATIS_DOXYFILE} ${${_exe_name}_DOCS_DIR}/DoxyfileTrac @ONLY)
	endif(ENABLE_${_exe_name}_TRAC_DOXYGEN)
	
	#4 remove existing documentation
	if(EXISTS html)
		file(GLOB HTML_FILES  ${${_exe_name}_DOCS_DIR}/html/*.html)
		remove(${HTML_FILES})
	endif(EXISTS html)
	if(EXISTS trac)
		file(GLOB TRAC_FILES  ${${_exe_name}_DOCS_DIR}/trac/*.html)
		remove(${TRAC_FILES})
	endif(EXISTS trac)


	#5 create custom target
	if(ENABLE_${_exe_name}_DOXYGEN_DOCS )
		add_custom_target(${_exe_name}_doxygen 
			COMMAND ${CMAKE_COMMAND} -E make_directory ${${_exe_name}_DOCS_DIR}/html
			COMMAND ${CMAKE_COMMAND} -E copy ${MATIS_DOXYFILE_DIR}/Header.html ${${_exe_name}_DOCS_DIR}
			COMMAND ${CMAKE_COMMAND} -E copy ${MATIS_DOXYFILE_DIR}/Footer.html ${${_exe_name}_DOCS_DIR}
			COMMAND ${DOXYGEN_EXECUTABLE} ${${_exe_name}_DOCS_DIR}/Doxyfile
			WORKING_DIRECTORY ${${_exe_name}_DOCS_DIR}
			DEPENDS ${${_exe_name}_DOCS_DIR}/Doxyfile)	
		add_dependencies(doc ${_exe_name}_doxygen)
		if(INSTALL_ALL_DOC)
			INSTALL (DIRECTORY ${${_exe_name}_DOCS_DIR}/html/ DESTINATION ${MATIS_DOC_DIR}/${_exe_name} )
			set(INSTALL_PATH ${INSTALL_PATH} ${MATIS_DOC_DIR}/${_exe_name})
		endif(INSTALL_ALL_DOC)
	endif(ENABLE_${_exe_name}_DOXYGEN_DOCS)
	
	if(ENABLE_${_exe_name}_TRAC_DOXYGEN)
		add_custom_target(${_exe_name}_trac_doxygen 
			COMMAND ${CMAKE_COMMAND} -E make_directory ${${_exe_name}_DOCS_DIR}/trac
			COMMAND ${CMAKE_COMMAND} -E copy ${MATIS_DOXYFILE_DIR}/TracHeader.html ${${_exe_name}_DOCS_DIR}
			COMMAND ${CMAKE_COMMAND} -E copy ${MATIS_DOXYFILE_DIR}/TracFooter.html ${${_exe_name}_DOCS_DIR}
			COMMAND ${DOXYGEN_EXECUTABLE} ${${_exe_name}_DOCS_DIR}/DoxyfileTrac
			WORKING_DIRECTORY ${${_exe_name}_DOCS_DIR}
			DEPENDS ${${_exe_name}_DOCS_DIR}/DoxyfileTrac )	
		if(CMAKE_VERBOSITY GREATER 1)
			MESSAGE(STATUS " .... copying   " ${MATIS_DOXYFILE_DIR}/TracHeader.html " to  " ${${_exe_name}_DOCS_DIR})
		endif(CMAKE_VERBOSITY GREATER 1)
		add_dependencies(doc ${_exe_name}_trac_doxygen )
		if(INSTALL_ALL_DOC)
			INSTALL (DIRECTORY ${${_exe_name}_DOCS_DIR}/trac/ DESTINATION ${MATIS_DOC_TRAC_DIR}/${_exe_name} )
			set(INSTALL_PATH ${INSTALL_PATH} ${MATIS_DOC_TRAC_DIR}/${_exe_name})
		endif(INSTALL_ALL_DOC)
	endif(ENABLE_${_exe_name}_TRAC_DOXYGEN)
	
	#6 install shared file and copy css/gif/png file on correct directories.
	__install_shared_documentation()
	
  endif(DOCUMENTATION_ENABLED)	
 if(CMAKE_VERBOSITY GREATER 1)
	MESSAGE(STATUS "--------- end documentation ---------"  ${exe_name})
endif(CMAKE_VERBOSITY GREATER 1)
endmacro(__add_documentation)


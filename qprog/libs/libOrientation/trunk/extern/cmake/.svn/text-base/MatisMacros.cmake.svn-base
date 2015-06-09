# Common Cmake Macros used in MATIS/Extern repository
# authors: Nicolas DAVID and Bruno VALLET
# date: 23/10/2009

####################################
### DEFAULT BUILD CONFIGURATION  ###
####################################
# Initialise and set variables global to the project
macro(__default_build_configuration)
	# project variables to export in the ${PROJECT_NAME}Config.cmake
	if(NOT ${PROJECT_NAME}_ROOT_DIR)
		if(${PROJECT_NAME}_IS_EXTERN)
			set(${PROJECT_NAME}_ROOT_DIR "${PROJECT_SOURCE_DIR}/..")
		else(${PROJECT_NAME}_IS_EXTERN)
			set(${PROJECT_NAME}_ROOT_DIR "${PROJECT_SOURCE_DIR}")
		endif(${PROJECT_NAME}_IS_EXTERN)
	endif(NOT ${PROJECT_NAME}_ROOT_DIR)
	set(${PROJECT_NAME}_DEFINITIONS "")
	set(${PROJECT_NAME}_INCLUDE_DIRS "")
	set(${PROJECT_NAME}_LIBRARIES "")
	if(NOT ${PROJECT_NAME}_URL)
		if(${PROJECT_NAME}_IS_EXTERN)
			set(${PROJECT_NAME}_URL "https://codes-matis/svn/extern/${PROJECT_NAME}")
		else(${PROJECT_NAME}_IS_EXTERN)
			set(${PROJECT_NAME}_URL "put_url_here")
		endif(${PROJECT_NAME}_IS_EXTERN)
	endif(NOT ${PROJECT_NAME}_URL)
	set(CMAKE_VERBOSITY 1 CACHE INT "0: none, 1: normal, 2: verbose")
	
	# build type
	if(UNIX AND NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "Release" CACHE STRING 
		" Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel.  "FORCE)
	endif(UNIX AND NOT CMAKE_BUILD_TYPE)

	# global definitions
	if (UNIX AND NOT APPLE)
		add_definitions(-D__LINUX__)
	endif (UNIX AND NOT APPLE)
	if (WIN32 AND MSVC)
		add_definitions(-D__WIN32__)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
		add_definitions(-D_SCL_SECURE_NO_WARNINGS)
	endif (WIN32 AND MSVC)

	# output directories
	if (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH
		" Where to put all the RUNTIME targets when built " FORCE)
	endif (NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)

	if (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib CACHE PATH 
		" Output directory in which to build LIBRARY target files "FORCE)
	endif (NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)

	if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
		set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib CACHE PATH
		" Where to put all the ARCHIVE targets when built " FORCE)
	endif (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)

	if (NOT EXECUTABLE_OUTPUT_PATH)
		set(EXECUTABLE_OUTPUT_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} CACHE PATH
		" old variable not use with cmake 2.6+ " FORCE)
	endif (NOT EXECUTABLE_OUTPUT_PATH)

	if (NOT LIBRARY_OUTPUT_PATH)
		set(LIBRARY_OUTPUT_PATH ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} CACHE PATH
		" old variable not use with cmake 2.6+ " FORCE)
	endif (NOT LIBRARY_OUTPUT_PATH)

	mark_as_advanced(
	CMAKE_ARCHIVE_OUTPUT_DIRECTORY
	EXECUTABLE_OUTPUT_PATH
	LIBRARY_OUTPUT_PATH)
endmacro(__default_build_configuration)

macro(__default_install_configuration)
	# install path
	if(UNIX)
		set(INSTALL_PREFIX "/usr/local" CACHE PATH " install path" )
	endif(UNIX)
	if(WIN32)
		set(INSTALL_PREFIX "C:/Program Files/MATIS" CACHE PATH " install path")
	endif(WIN32)
	set(CMAKE_INSTALL_PREFIX "${INSTALL_PREFIX}" CACHE INTERNAL " real install path" FORCE)
	
	# documentation install default path
	if(UNIX)
		set(MATIS_DOC_DIR "/usr/local/doc" CACHE PATH " install path" )
	endif(UNIX)
	if(WIN32)
		set(MATIS_DOC_DIR "C:/Program Files/MATIS/doc" CACHE PATH " install path")
	endif(WIN32)
	SET(MATIS_DOC_COMMON_DIR ${MATIS_DOC_DIR}/matis_doc_template )
	SET(MATIS_DOC_TRAC_DIR ${MATIS_DOC_DIR}/trac )
	
	# prefix path (mainly used to tell find_package where to look)
	set(PREFIX_PATH "${INSTALL_PREFIX}" CACHE PATHS " where find_package looks")
	set(CMAKE_PREFIX_PATH "${PREFIX_PATH}" CACHE INTERNAL " where find_package looks" FORCE)
endmacro(__default_install_configuration)

macro(__general_output)
	# output general project info
	if(CMAKE_VERBOSITY GREATER 0)
		message(STATUS "---------------- GENERAL --------------------")
		message(STATUS "- Project ${PROJECT_NAME} under ${CMAKE_SYSTEM}")
		if(CMAKE_BUILD_TYPE)
		message(STATUS "- Build type: ${CMAKE_BUILD_TYPE}")
		else(CMAKE_BUILD_TYPE)
		message(STATUS "- Debug/Release/... mode will be specified by the environment")
		endif(CMAKE_BUILD_TYPE)
		message(STATUS "- Project root: ${${PROJECT_NAME}_ROOT_DIR}")
		message(STATUS "- Makefile/solutions are in: ${CMAKE_BINARY_DIR}")
		message(STATUS "- Executables are built in: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
		message(STATUS "- Libraries are built in: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
		message(STATUS "- Project is installed in: ${CMAKE_INSTALL_PREFIX}")
		if(${${PROJECT_NAME}_IS_COMPONENT})
			message(STATUS "- Project has multiple components")
		endif(${${PROJECT_NAME}_IS_COMPONENT})
		if(CMAKE_VERBOSITY GREATER 1)
			if(CMAKE_BUILD_TYPE)
				string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_CMAKE_BUILD_TYPE)
				message(STATUS "- Default cxx flags are: ${CMAKE_CXX_FLAGS_${UPPER_CMAKE_BUILD_TYPE}}")
			else(CMAKE_BUILD_TYPE)
				message(STATUS "- Default cxx flags are: ${CMAKE_CXX_FLAGS}")
			endif(CMAKE_BUILD_TYPE)
		endif(CMAKE_VERBOSITY GREATER 1)
		message(STATUS "---------------------------------------------")
	endif(CMAKE_VERBOSITY GREATER 0)
	
endmacro(__general_output)

#######################
### DECLARE TARGETS ###
#######################
# check for available ${PROJECT_NAME}-lib.cmake files, includes them according to user preferences
macro(__declare_targets)
	option(CUSTOM_BUILD "Manually choose what to build. If not set, build only ${PROJECT_NAME} libraries" OFF)
	
	# libraries
	if(EXISTS "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-lib.cmake")
		if(CUSTOM_BUILD)
			option(BUILD_${PROJECT_NAME}_LIBRARY "Build ${PROJECT_NAME} libraries " ON)
		else(CUSTOM_BUILD)
			set(BUILD_${PROJECT_NAME}_LIBRARY ON)
		endif(CUSTOM_BUILD)
		if(BUILD_${PROJECT_NAME}_LIBRARY)
			if(CUSTOM_BUILD)
				option(BUILD_SHARED_LIBS "Build ${PROJECT_NAME} libraries shared." ON)
			else(CUSTOM_BUILD)
				set(BUILD_SHARED_LIBS ON)
			endif(CUSTOM_BUILD)
			include("${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-lib.cmake")
		endif(BUILD_${PROJECT_NAME}_LIBRARY)
	endif(EXISTS "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-lib.cmake")
	
	# executables
	if(EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-bin.cmake)
		if(CUSTOM_BUILD)
			option(BUILD_${PROJECT_NAME}_EXE "Build ${PROJECT_NAME} executables" OFF)
		else(CUSTOM_BUILD)
			set(BUILD_${PROJECT_NAME}_EXE OFF)
		endif(CUSTOM_BUILD)
		if(BUILD_${PROJECT_NAME}_EXE)
			include(${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-bin.cmake)
		endif(BUILD_${PROJECT_NAME}_EXE)
	endif(EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-bin.cmake)
	
	# tests
	if(EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-test.cmake)
		if(CUSTOM_BUILD)
			option(RUN_${PROJECT_NAME}_TESTS "Run some ${PROJECT_NAME} .sh test scripts" OFF)
		else(CUSTOM_BUILD)
			set(RUN_${PROJECT_NAME}_TESTS OFF)
		endif(CUSTOM_BUILD)
		if(RUN_${PROJECT_NAME}_TESTS)
			enable_testing()
			include(${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-test.cmake)
			message(STATUS "Added tests")
		endif(RUN_${PROJECT_NAME}_TESTS)
	endif(EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-test.cmake)

	# documentation
	if(EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-docs.cmake)
		if(CUSTOM_BUILD)
			option(BUILD_${PROJECT_NAME}_DOCUMENTATION "Build ${PROJECT_NAME} documentation" OFF)
		else(CUSTOM_BUILD)
			set(BUILD_${PROJECT_NAME}_DOCUMENTATION OFF)
		endif(CUSTOM_BUILD)
		if(BUILD_${PROJECT_NAME}_DOCUMENTATION)
			IF(NOT TARGET doc)
				add_custom_target(doc)
	        ENDIF(NOT TARGET doc)
			include(${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-docs.cmake)
			message(STATUS "Added docs. Don't forget to make the docs before installing.")
		endif(BUILD_${PROJECT_NAME}_DOCUMENTATION)
	endif(EXISTS ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}-docs.cmake)

endmacro(__declare_targets)

#######################
### INSTALL TARGETS ###
#######################
macro(__install_targets)

	# install xxx-targets.cmake files needed to import targets)
	if(${PROJECT_NAME}_HAS_TARGETS)
		install (EXPORT ${PROJECT_NAME}-targets DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake")
	endif(${PROJECT_NAME}_HAS_TARGETS)

	# install xxxConfig.cmake files needed to make FIND_PACKAGE(xxx) find this project
	# requires the following variables to be properly defined (configure_file does not accept mixing @ and $)
	set(_DEFINITIONS ${${PROJECT_NAME}_DEFINITIONS})
	set(_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS})
	set(_LIBRARIES ${${PROJECT_NAME}_LIBRARIES})
	set(_LIBRARY ${${PROJECT_NAME}_LIBRARY})
	set(_URL ${${PROJECT_NAME}_URL})
	# if project is on extern directories get the cmake config file from the ../../cmake directory and install it
	if(${PROJECT_NAME}_IS_EXTERN)
		set( MATIS_CLASSIC_CONFIG "../../cmake/MatisConfig.cmake.in")
		set( MATIS_COMPONENT_CONFIG "../../cmake/ComponentConfig.cmake.in" )
		# externs always install the matis macros
		install (FILES "../../cmake/MatisMacros.cmake" 
						"../../cmake/MatisConfig.cmake.in" 
						"../../cmake/MatisDoxygenMacros.cmake" 
						"../../cmake/ComponentConfig.cmake.in"
						"../../cmake/modules/FindLib3DS.cmake"  
				DESTINATION "${CMAKE_ROOT}/Modules")
		install (DIRECTORY "../../cmake/doxygen"  DESTINATION ${MATIS_DOC_COMMON_DIR} REGEX "/.svn*" EXCLUDE)		
	else(${PROJECT_NAME}_IS_EXTERN)
		set( MATIS_CLASSIC_CONFIG "${CMAKE_ROOT}/Modules/MatisConfig.cmake.in")
		set( MATIS_COMPONENT_CONFIG "${CMAKE_ROOT}/Modules/ComponentConfig.cmake.in")
	endif(${PROJECT_NAME}_IS_EXTERN)
	# generate cmake mytargetConfig.cmake file. two possible case, project is only a library or is a set of component libraries
	if( ${PROJECT_NAME}_IS_COMPONENT)
		configure_file("${MATIS_COMPONENT_CONFIG}" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
	else( ${PROJECT_NAME}_IS_COMPONENT)
		configure_file("${MATIS_CLASSIC_CONFIG}" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
	endif( ${PROJECT_NAME}_IS_COMPONENT)
  #install the configuration file
	install (FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake")
	
endmacro(__install_targets)

#############################################
### BUILD A STANDARD MATIS EXTERN PROJECT ###
#############################################
# Matis Extern projects have an out of source cmake files arborescence
# Matis Extern call the macros from extern/cmake and installs them in CMake modules
macro(__matis_extern_project)
	#configuration step
	set(${PROJECT_NAME}_IS_EXTERN 1)
	if(NOT ${PROJECT_NAME}_IS_COMPONENT)
		set(${PROJECT_NAME}_IS_COMPONENT OFF)
	endif(NOT ${PROJECT_NAME}_IS_COMPONENT)
	#call other macro
	__default_build_configuration()
	__default_install_configuration()
	__general_output()
	__declare_targets()
	__install_targets()
endmacro(__matis_extern_project)

######################################
### BUILD A STANDARD MATIS PROJECT ###
######################################
# Matis projects have an in source cmake files arborescence
# Matis call the macros installed in CMake modules (by any extern)
macro(__matis_project _src_dirs)
	__default_build_configuration()
	__default_install_configuration()
	__general_output()
	add_subdirectory(${_src_dirs})
	__install_targets()
endmacro(__matis_project)

# for each package XXX, FIND_PACKAGE defines (or not)
#  XXX_FOUND, If false, do not try to use XXX.
#  XXX_INCLUDE_DIR(S), where to find xxx.h, etc.
#  XXX_LIBRARIES, the libraries to link against when using XXX
#  XXX_DEFINITIONS - Compiler switches required for using XXX
# this is handled in the following macros

########################
### IMPORT VARIABLES ###
########################
macro(__import_variables _from _to)
	string(TOUPPER ${_from} FROM)
	set(_USE_FILE 0)
	if(${_from}_USE_FILE)
		include (${${_from}_USE_FILE})
		message(STATUS "Using ${${_from}_USE_FILE}")
		set(_USE_FILE 1)
	endif(${_from}_USE_FILE)
	if(NOT _USE_FILE AND ${FROM}_USE_FILE)
		include (${${FROM}_USE_FILE})
		message(STATUS "Using ${${FROM}_USE_FILE}")
		set(_USE_FILE 1)
	endif(NOT _USE_FILE AND ${FROM}_USE_FILE)
	if(NOT _USE_FILE)
		set(${_to}_DEFINITIONS ${${_to}_DEFINITIONS} ${${_from}_DEFINITIONS} ${${FROM}_DEFINITIONS})
		set(${_to}_INCLUDE_DIRS ${${_to}_INCLUDE_DIRS} ${${_from}_INCLUDE_DIRS} ${${FROM}_INCLUDE_DIRS})
	endif(NOT _USE_FILE)
	set(${_to}_LIBRARIES ${${_to}_LIBRARIES} ${${_from}_LIBRARY} ${${_from}_LIBRARIES} ${${FROM}_LIBRARY} ${${FROM}_LIBRARIES})
endmacro(__import_variables)

###########################
### HANDLE DEPENDENCIES ###
###########################
macro(__handle_dependencies _name _dep)
	set(DEPENDENCIES_PRESENT 1)
	foreach(PKG ${${_dep}})
		string(TOUPPER ${PKG} UPPER_PKG)
		# 1 - look for an intern target named ${PKG}
		if(TARGET ${PKG})
			set(_${_name}_LIBRARIES ${_${_name}_LIBRARIES} ${PKG})
			if(CMAKE_VERBOSITY GREATER 1)
				message(STATUS "${_name} depends on target ${PKG}")
			endif(CMAKE_VERBOSITY GREATER 1)
			set(${PKG}_FOUND 1)
		else(TARGET ${PKG})
			# 2 - look for a previously found package named ${PKG}
			if(${PKG}_FOUND)
				__import_variables(${PKG} _${_name})
				if(CMAKE_VERBOSITY GREATER 1)
					message(STATUS "${_name} depends on previously found package ${PKG}")
				endif(CMAKE_VERBOSITY GREATER 1)
			endif(${PKG}_FOUND)
		endif(TARGET ${PKG})
		
		# 3 - look for a ${PKG}Config.cmake in the installation directory
		if(NOT ${PKG}_FOUND)
			find_package(${PKG} NO_MODULE QUIET)
			#include("${CMAKE_INSTALL_PREFIX}/cmake/${PKG}Config.cmake" OPTIONAL)
			if(${PKG}_FOUND)
				__import_variables(${PKG} _${_name})
				if(CMAKE_VERBOSITY GREATER 1)
					message(STATUS "${_name} depends on package ${PKG} found in ${CMAKE_INSTALL_PREFIX}")
				endif(CMAKE_VERBOSITY GREATER 1)				
			endif(${PKG}_FOUND )
		endif(NOT ${PKG}_FOUND)
		
		# 4 - look for a Find${PKG}.cmake in CMAKE_MODULE_PATH
		if(NOT ${PKG}_FOUND)
			find_package(${PKG})
			if(${PKG}_FOUND)
				__import_variables(${PKG} _${_name})
				if(CMAKE_VERBOSITY GREATER 1)
					message(STATUS "${_name} depends on package ${PKG} found on this computer")
				endif(CMAKE_VERBOSITY GREATER 1)
			endif(${PKG}_FOUND)
		endif(NOT ${PKG}_FOUND)		
		
		# 5 - if you really can't find anything named ${PKG}
		if(NOT ${PKG}_FOUND)
			set(DEPENDENCIES_PRESENT 0)
			message(ERROR " ${PKG} is missing for ${_name}")
			if(CMAKE_VERBOSITY GREATER 1)
				message(STATUS "If ${PKG} is a target, check that you have added or imported it before adding ${_name}")
				message(STATUS "If ${PKG} is a package from MATIS/Extern, check that you have installed it in ${CMAKE_INSTALL_PREFIX}")
				message(STATUS "If ${PKG} is a package installed on your computer set PREFIX_PATH to its location")
				message(STATUS " or switch to advanced view and set ${UPPER_PKG}_INCLUDE_DIR and ${UPPER_PKG}_LIBRARY")
			endif(CMAKE_VERBOSITY GREATER 1)
			# I guess ND is behind that, ask him why he did that
			if(UNIX AND CMAKE_VERBOSITY LESS 2)
				set(${PKG}_DIR "${CMAKE_INSTALL_PREFIX}/cmake" CACHE INTERNAL " normal directory of cmake config file" FORCE)
			endif(UNIX AND CMAKE_VERBOSITY LESS 2)
		endif(NOT ${PKG}_FOUND)
	
	endforeach(PKG ${${_depends}})	
	
endmacro(__handle_dependencies)

#########################
### ADD AN EXECUTABLE ###
#########################
# _source and _header are lists of source and header files with full path 
# You can use file(GLOB ...) and the __global_append macro to simplify this
# _def contains all precompiler definitions AND C/CXX flags
# _dep contains all intern targets AND all packages that the exe depends on
macro(__add_executable _exe_name _source _header _def _dep)

	# In case of a custom build, allow user to choose wether or not building the executable
	if(CUSTOM_BUILD)
		option(BUILD_${_exe_name} "Build ${_exe_name} executable" ON)
	else(CUSTOM_BUILD)
		set(BUILD_${_exe_name} ON)
	endif(CUSTOM_BUILD)
	
	if(BUILD_${_exe_name})
		# Handle dependencies
		set(_${_exe_name}_DEFINITIONS ${${_def}})
		__handle_dependencies(${_exe_name} ${_dep})

		if(DEPENDENCIES_PRESENT)			
			if(CMAKE_VERBOSITY GREATER 1)
				message(STATUS _${_exe_name}_DEFINITIONS = ${_${_exe_name}_DEFINITIONS})
				message(STATUS _${_exe_name}_INCLUDE_DIRS = ${_${_exe_name}_INCLUDE_DIRS})
				message(STATUS _${_exe_name}_LIBRARIES = ${_${_exe_name}_LIBRARIES})
			endif(CMAKE_VERBOSITY GREATER 1)
			
			# add include dirs from dependancies	
			include_directories(${_${_exe_name}_INCLUDE_DIRS})
			
			# add and link executable
			add_executable(${_exe_name} ${${_source}} ${${_header}})
			target_link_libraries(${_exe_name} ${_${_exe_name}_LIBRARIES})
			# add_dependencies(${_exe_name} ${_${_exe_name}_LIBRARIES}) TODO: separate intern targets from imported libs
			
			# add flags and definitions from dependencies and from manual input
			set(${_exe_name}_EXE_CXX_FLAGS "" CACHE STRING
				"Additional Definitions and Flags to use when compiling ${_exe_name} executable")
			mark_as_advanced(${_exe_name}_EXE_CXX_FLAGS)
			# set_property(TARGET ${_exe_name} APPEND PROPERTY COMPILE_FLAGS ${_${_exe_name}_DEFINITIONS} ${${_exe_name}_EXE_CXX_FLAGS})
			add_definitions(${_${_exe_name}_DEFINITIONS} ${${_exe_name}_EXE_CXX_FLAGS})
			
			# install
			INSTALL (TARGETS ${_exe_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
			
			# Handle status output
			if(CMAKE_VERBOSITY GREATER 0)
				message(STATUS "Added executable ${_exe_name}")
			endif(CMAKE_VERBOSITY GREATER 0)
		endif(DEPENDENCIES_PRESENT)
	endif(BUILD_${_exe_name})
endmacro(__add_executable)

#####################
### ADD A LIBRARY ###
#####################
# _source and _header are lists of source and header files with full path 
# You can use file(GLOB ...) and the __global_append macro to simplify this
# _def contains all precompiler definitions AND C/CXX flags
# _dep contains all intern targets AND all packages that the lib depends on
macro(__add_library _lib_name _source _header _def _dep)
	# Switch between static or shared library
	if(BUILD_SHARED_LIBS)
		set(_${_lib_name}_SHARED ON)
		set(_${_lib_name}_LIBRARY_TYPE SHARED)
	else(BUILD_SHARED_LIBS)
		set(_${_lib_name}_SHARED OFF)
		set(_${_lib_name}_LIBRARY_TYPE STATIC)
	endif(BUILD_SHARED_LIBS)

	# Handle dependencies
	set(_${_lib_name}_DEFINITIONS ${${_def}})
	__handle_dependencies(${_lib_name} ${_dep})

	if(DEPENDENCIES_PRESENT)		
		# add definitions and include dirs from dependencies		
		include_directories(${_${_lib_name}_INCLUDE_DIRS})
		if(CMAKE_VERBOSITY GREATER 1)
			message(STATUS _${_lib_name}_DEFINITIONS = ${_${_lib_name}_DEFINITIONS})
			message(STATUS _${_lib_name}_INCLUDE_DIRS = ${_${_lib_name}_INCLUDE_DIRS})
			message(STATUS _${_lib_name}_LIBRARIES = ${_${_lib_name}_LIBRARIES})
		endif(CMAKE_VERBOSITY GREATER 1)
		# add and link library
		add_library(${_lib_name} ${_${_lib_name}_LIBRARY_TYPE} ${${_source}} ${${_header}})
		target_link_libraries(${_lib_name} ${_${_lib_name}_LIBRARIES})
		set_target_properties(${_lib_name} PROPERTIES DEBUG_POSTFIX "-d")
		# add_dependencies(${_lib_name} ${_${_lib_name}_LIBRARIES}) TODO: separate intern targets from imported libs
		
		# add flags and definitions from dependencies and from manual input
		set(${_lib_name}_LIB_CXX_FLAGS "" CACHE STRING
			"Additional Definitions and Flags to use when compiling ${_lib_name} library")
		mark_as_advanced(${_lib_name}_LIB_CXX_FLAGS)
		# set_property(TARGET ${_lib_name} APPEND PROPERTY COMPILE_FLAGS ${_${_lib_name}_DEFINITIONS} ${${_lib_name}_LIB_CXX_FLAGS})
		add_definitions(${_${_lib_name}_DEFINITIONS} ${${_lib_name}_LIB_CXX_FLAGS})
		
		### INSTALL ###
		#if(NOT TARGET ${_lib_name})
		# headers (install .h files to include)
		if(FORCE_INCLUDE_DIR)
			set(INSTALL_INC_DIR ${FORCE_INCLUDE_DIR})
		else(FORCE_INCLUDE_DIR)
			set(INSTALL_INC_DIR ${CMAKE_INSTALL_PREFIX}/include/${_lib_name})
		endif(FORCE_INCLUDE_DIR)
		install (FILES ${${_header}} DESTINATION "${INSTALL_INC_DIR}/")
		# library (install .lib/.dll/.o/.so lib files)
		install(
			TARGETS ${_lib_name}
			EXPORT ${PROJECT_NAME}-targets
			ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
			LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
			RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
		)
		set(${PROJECT_NAME}_HAS_TARGETS 1)
		
		# push the lib variables to the package variables
		# this is used to collect variables to put in ${PACKAGE}Config.cmake (called by find_package)
		# recursivity is not handled anymore
		# if you want _DEFINITIONS, _INCLUDE_DIRS or _LIBRARIES from a sub-lib, find_package it
		# TODO: handle the COMPONENTS in the ${PACKAGE}Config.cmake
		set(${PROJECT_NAME}_DEFINITIONS ${${PROJECT_NAME}_DEFINITIONS} ${${_def}})
		set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} ${INSTALL_INC_DIR})
		set(${PROJECT_NAME}_LIBRARIES ${${PROJECT_NAME}_LIBRARIES} ${_lib_name})
                set(${PROJECT_NAME}_LIBRARY ${_lib_name})
		# Handle status output
		if(CMAKE_VERBOSITY GREATER 0)
			message(STATUS "Added library ${_lib_name} (${_${_lib_name}_LIBRARY_TYPE})")
		endif(CMAKE_VERBOSITY GREATER 0)
	endif(DEPENDENCIES_PRESENT)

endmacro(__add_library)

################################
### ADD HEADERS ONLY LIBRARY ###
################################

macro(__add_headers_only_library _lib_name _header _def)
	#handle definitions
	foreach(DEF ${${_def}})
		add_definitions(-D${DEF})
	endforeach(DEF ${${_def}})

	### INSTALL headers
	set(INSTALL_INC_DIR ${CMAKE_INSTALL_PREFIX}/include/${_lib_name})
	install (FILES ${${_header}} DESTINATION "${INSTALL_INC_DIR}/")
	
	# push the lib variables to the package variables
	set(${PROJECT_NAME}_DEFINITIONS ${${PROJECT_NAME}_DEFINITIONS} ${${_def}})
	set(${PROJECT_NAME}_INCLUDE_DIRS ${${PROJECT_NAME}_INCLUDE_DIRS} ${INSTALL_INC_DIR})

	# Handle status output
	if(CMAKE_VERBOSITY GREATER 0)
		message(STATUS "Added headers only library ${_lib_name}")
	endif(CMAKE_VERBOSITY GREATER 0)
endmacro(__add_headers_only_library)

#####################
### GLOBAL APPEND ###
#####################
# append something in front of each element of a list
# helpful in the ${PACKAGE_NAME}-xxx.cmake when declaring sources and headers with full path
macro(__global_append _from _to _append)
	set(${_to} "")
	foreach(var ${${_from}})
		set(${_to} ${${_to}} "${_append}${var}")
	endforeach(var ${${_from}})
endmacro(__global_append)

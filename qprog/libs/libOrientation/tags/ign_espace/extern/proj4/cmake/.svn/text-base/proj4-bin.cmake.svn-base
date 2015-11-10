# adds PROJ4 executable targets
# authors: Nicolas DAVID and Bruno VALLET
# date: 23/10/2009

if(NOT BUILD_PROJ4_LIBRARY)
	message(ERROR " you need to compile PROJ4 library to compile the programs" ) 
endif(NOT BUILD_PROJ4_LIBRARY)

if(CUSTOM_BUILD)
	option(BUILD_CS2CS "Build cs2cs (coordinate systems to coordinate systems translation tool)" OFF)
	option(BUILD_PROJ "Build proj (cartographic projection tool : latlong <-> projected coordinates" OFF)
	option(BUILD_GEOD "Build geod (computation of geodesic lines)" OFF)
	option(BUILD_NAD2NAD "Build nad2nad (format conversion tool)" OFF)
	option(BUILD_NAD2BIN "Build nad2bin (format conversion tool)" OFF)
else(CUSTOM_BUILD)
	set(BUILD_CS2CS ON)
	set(BUILD_PROJ ON)
	set(BUILD_GEOD ON)
	set(BUILD_NAD2NAD ON)
	set(BUILD_NAD2BIN ON)
endif(CUSTOM_BUILD)

if(CONVERT_DATA)
	set(BUILD_NAD2BIN ON) #PARENT_SCOPE)
endif(CONVERT_DATA)

if(BUILD_NAD2NAD AND WIN32 AND BUILD_SHARED_LIBS)
	message(warning " nad2nad can't be build with a DLL proj4 library you need a static lib")
	set(BUILD_NAD2NAD OFF CACHE BOOL "Build nad2nad (format conversion tool)" FORCE)
endif(BUILD_NAD2NAD AND WIN32 AND BUILD_SHARED_LIBS)

set(CS2CS_SRC cs2cs.c gen_cheb.c p_series.c)
set(CS2CS_HEADERS "")
set(PROJ_SRC proj.c gen_cheb.c p_series.c )
set(PROJ_HEADERS "")
set(GEOD_SRC geod.c geod_set.c geod_for.c geod_inv.c )
set(GEOD_HEADERS geodesic.h)
set(NAD2NAD_SRC nad2nad.c )
set(NAD2NAD_HEADERS "")
set(NAD2BIN_SRC nad2bin.c )
set(NAD2BIN_HEADERS "")

include_directories(${PROJ4_ROOT_DIR}/src)

set(DEPENDENCIES proj4)
if(UNIX)
	set(DEPENDENCIES ${DEPENDENCIES} m)
endif(UNIX)

macro(__add_proj4_executable _exe_name)
	string(TOUPPER ${_exe_name} EXE_NAME)
	string(TOLOWER ${_exe_name} exe_name)
	if(BUILD_${EXE_NAME})
		if(WIN32)
			set(${EXE_NAME}_SRC ${${EXE_NAME}_SRC} emess.c)
		endif(WIN32)
		__global_append(${EXE_NAME}_SRC FULL_SOURCES "${PROJ4_ROOT_DIR}/src/")
		__global_append(${EXE_NAME}_HEADERS FULL_HEADERS "${PROJ4_ROOT_DIR}/src/")
		add_executable(${exe_name} ${FULL_SOURCES} ${FULL_HEADERS})
		target_link_libraries(${exe_name} ${DEPENDENCIES})
		install (TARGETS ${exe_name} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
		message(STATUS "Added executable ${_exe_name}")
	endif(BUILD_${EXE_NAME})
endmacro(__add_proj4_executable)

set(EXE_LIST cs2cs proj geod nad2nad nad2bin)
foreach(exe ${EXE_LIST})
	__add_proj4_executable (${exe})
endforeach(exe ${EXE_LIST})



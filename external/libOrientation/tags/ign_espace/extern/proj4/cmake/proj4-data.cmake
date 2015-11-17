# adds PROJ4 data
# authors: Nicolas DAVID and Bruno VALLET
# date: 23/10/2009

if(CONVERT_DATA AND NOT BUILD_NAD2BIN)
	message(ERROR " you need to compile nad2bin program in order to convert data file" )
endif(CONVERT_DATA AND NOT BUILD_NAD2BIN)

set(DATA_DIR ${proj4_ROOT_DIR}/nad)
###############################
### DATA CONVERSION SUFF     ###
################################
if(CONVERT_DATA)
	#set(NAD2BIN_DIR ${PROJECT_BINARY_DIR}/bin)
	set(NAD2BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_CFG_INTDIR})
	set(NAD2BIN_PATH ${NAD2BIN_DIR}/nad2bin${CMAKE_EXECUTABLE_SUFFIX})
	file(TO_NATIVE_PATH ${NAD2BIN_PATH} NAD2BIN_EXE)
	file(GLOB LLA_FILES  ${DATA_DIR}/*.lla)
	foreach(FILE ${LLA_FILES} )
		 get_filename_component(filename ${FILE} NAME_WE)
		 get_filename_component(pathname ${FILE} PATH)
		 set(DEST_DIR ${pathname}/${filename})
		 file(TO_NATIVE_PATH ${DEST_DIR} DEST)
		 if( NOT EXISTS ${DEST})
		 	add_custom_target( ${filename} ALL
					COMMAND ${NAD2BIN_EXE} ${DEST} "<" ${FILE}
					DEPENDS nad2bin
				)
		endif(NOT EXISTS ${DEST})
	endforeach(FILE)
endif(CONVERT_DATA)

#######################################
### DEFINE DATA FILE VARIABLES     ###
#######################################
# files containing definition of useful projection
# ie : epsg , IGNF, etc..
set( DATA_DEF_PROJECTION
	${DATA_DIR}/proj_def.dat # global file for world projection definiton
	${DATA_DIR}/epsg
	${DATA_DIR}/esri
	${DATA_DIR}/world
	${DATA_DIR}/esri.extra
	${DATA_DIR}/other.extra
	${DATA_DIR}/IGNF # french ign data file
	${DATA_DIR}/IGNVD
	${DATA_DIR}/nad27 # USA data file
	${DATA_DIR}/nad83
	${DATA_DIR}/nad.lst 
)
# gridshift file
# all .gsb on data directory + other
file(GLOB GSB_FILES  ${DATA_DIR}/*.gsb)
set( GRIDSHIFT_FILES ${GSB_FILES})
file(GLOB LLA_FILES  ${DATA_DIR}/*.lla)
foreach(FILE ${LLA_FILES} )
		 get_filename_component(filename ${FILE} NAME_WE)
		 get_filename_component(pathname ${FILE} PATH)
		 set(DEST_DIR ${pathname}/${filename})
		 #file(TO_NATIVE_PATH ${DEST_DIR} DEST) 
		 set(DEST ${DEST_DIR})
		 if( EXISTS ${DEST})
			set( GRIDSHIFT_FILES ${GRIDSHIFT_FILES} ${DEST} )
		 endif(EXISTS ${DEST})
endforeach(FILE)
set( GRIDSHIFT_FILES ${GRIDSHIFT_FILES} ${DATA_DIR}/ntv1_can.dat )


# geoid file
# all .gra and .bin on data  directory
file(GLOB GRA_FILES  ${DATA_DIR}/*.gra)
file(GLOB BIN_FILES  ${DATA_DIR}/*.bin)
set( GEOID_FILES ${GRA_FILES} ${BIN_FILES})

#######################################
### INSTALL						     ###
#######################################
if(INSTALL_DATA)
	install (FILES ${DATA_DEF_PROJECTION} ${GRIDSHIFT_FILES} ${GEOID_FILES} 
			DESTINATION ${CMAKE_INSTALL_PREFIX}/share/proj4
			PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE)
endif(INSTALL_DATA)

# Installs Orientation library on the system

####
#### Install all ".h", ".hpp", ".hxx" and ".inl" files
####
FILE( GLOB ALL_HEADER_FILES src/*.h src/*.hxx src/*.hpp )
#MESSAGE(FATAL_ERROR ${ALL_HEADER_FILES} )
set(INSTALL_INC_DIR ${CMAKE_INSTALL_PREFIX}/include/Orientation)
install (FILES ${ALL_HEADER_FILES} DESTINATION "${INSTALL_INC_DIR}/")

FILE( GLOB ALL_CONFIG_FILES config/*.h config/*.hxx config/*.hpp )
install (FILES ${ALL_CONFIG_FILES} DESTINATION "${INSTALL_INC_DIR}/config")
		
#  export Orientation target
install(
TARGETS Orientation
EXPORT Orientation-targets
ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)
	
# install xxx-targets.cmake files needed to import targets
install (EXPORT Orientation-targets DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

# install xxxConfig.cmake files needed to make FIND_PACKAGE(xxx) find this project
# requires the following variables to be properly defined (configure_file does not accept mixing @ and $)
set(_DEFINITIONS "")
set(_INCLUDE_DIRS ${INSTALL_INC_DIR} "${INSTALL_INC_DIR}/config")
set(_LIBRARIES Orientation ${Orientation_LIBRARIES})
configure_file(extern/cmake/MatisConfig.cmake.in ${PROJECT_BINARY_DIR}/OrientationConfig.cmake @ONLY)
install (FILES ${PROJECT_BINARY_DIR}/OrientationConfig.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)

 

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT( Orientation )

SET( SRC_DIR src/ )
AUX_SOURCE_DIRECTORY( ${SRC_DIR}  SRC_ROOT )

OPTION( BUILD_Orientation_SHARED "Build Orientation as a SHARED library." OFF )
IF( BUILD_Orientation_SHARED )
	SET( ORIENTATION_LIBRARY_TYPE SHARED )
ELSE( BUILD_Orientation_SHARED )
	SET( ORIENTATION_LIBRARY_TYPE STATIC )
ENDIF( BUILD_Orientation_SHARED )

# SET classical path for the MATIS library
#
IF(UNIX)
	SET(CMAKE_PREFIX_PATH "/usr/local" ${CMAKE_PREFIX_PATH}  )
ENDIF(UNIX)
IF(WIN32)
	SET(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "C:/Program Files/MATIS" )
ENDIF(WIN32)
#
##############################################################################
# BOOST
##############################################################################
SET( Boost_USE_STATIC_LIBS OFF )
SET( Boost_USE_MULTITHREAD ON )
FIND_PACKAGE( Boost 1.36 COMPONENTS system filesystem REQUIRED )
SET(Orientation_LIBRARIES ${Boost_LIBRARIES})
##############################################################################
# Fin BOOST
##############################################################################

##############################################################################
# TinyXML
##############################################################################
# Option pour selectionner une compilation sous forme de .lib ou de .dll
OPTION(BUILD_TINYXML_SHARED "Build tinyxml library shared." OFF)

# Verification du type de bibliotheque a construire
IF(BUILD_TINYXML_SHARED)
  SET(TINYXML_LIBRARY_TYPE SHARED)
ELSE(BUILD_TINYXML_SHARED)
  SET(TINYXML_LIBRARY_TYPE STATIC)
ENDIF(BUILD_TINYXML_SHARED)

# On fixe une variable globale regroupant tous les fichiers source de la shapelib
SET( SRC_TINYXMLLIB extern/tinyxml )
SET( ALL_TINYXML_SRC_FILES ${SRC_TINYXMLLIB}/tinystr.cpp
						   ${SRC_TINYXMLLIB}/tinystr.h
						   ${SRC_TINYXMLLIB}/tinyxml.cpp
						   ${SRC_TINYXMLLIB}/tinyxml.h
						   ${SRC_TINYXMLLIB}/tinyxmlerror.cpp
						   ${SRC_TINYXMLLIB}/tinyxmlparser.cpp )
		   
# On ajoute les repertoires d'include
INCLUDE_DIRECTORIES( ${SRC_TINYXMLLIB} )

# Repertoire de sortie
ADD_LIBRARY( tinyxml ${TINYXML_LIBRARY_TYPE} ${ALL_TINYXML_SRC_FILES} )
SET(Orientation_LIBRARIES ${Orientation_LIBRARIES} tinyxml)

##############################################################################
# Fin TinyXML
##############################################################################

# Option pour utiliser xerces
OPTION(USE_XERCES "" OFF)

# Verification du type de bibliotheque a construire
IF(USE_XERCES)
  ADD_DEFINITIONS(-DORIENTATIONMATIS_USE_XERCES=1)
ELSE(USE_XERCES)
  ADD_DEFINITIONS(-DORIENTATIONMATIS_USE_XERCES=0)
ENDIF(USE_XERCES)



##############################################################################
# Orientation
##############################################################################

ADD_LIBRARY( Orientation  ${ORIENTATION_LIBRARY_TYPE}  ${SRC_ROOT} )
#ADD_DEPENDENCIES(	Orientation tinyxml proj4)
ADD_DEPENDENCIES(	Orientation tinyxml)

##############################################################################
# Fin Orientation
##############################################################################

##############################################################################
# PROJ4
##############################################################################
find_package(proj4 QUIET)
IF(proj4_FOUND)
	SET(PROJ4_INSTALL_DIR "proj4")
	include_directories(${proj4_INCLUDE_DIRS})
ELSE(proj4_FOUND)
	SET(ROOT_DIR ${PROJECT_SOURCE_DIR})
	SET(PROJ4_INSTALL_DIR "proj4")
	SET(CUSTOM_BUILD ON)
	SET(PROJ4_ROOT_DIR ${ROOT_DIR}/extern/proj4)
	include( ${ROOT_DIR}/extern/proj4/cmake/libproj4/CMakeLists.txt )
	ADD_DEPENDENCIES(Orientation proj4)
ENDIF(proj4_FOUND)

ADD_DEFINITIONS(-DORIENTATIONMATIS_USE_PROJ4=1)
SET( Orientation_LIBRARIES ${Orientation_LIBRARIES} proj4)

##############################################################################
# Fin PROJ4
##############################################################################

TARGET_LINK_LIBRARIES( Orientation ${Orientation_LIBRARIES})

##############################################################################
# Executables de test
##############################################################################
ENABLE_TESTING()
#SET	( ENV{PROJ_LIB}  /home/dboldo/noyau/codes-matis/libOrientation/trunk/extern/proj4/nad)
#SET (CTEST_ENVIRONMENT
#	"PROJ_LIB=/home/dboldo/noyau/codes-matis/libOrientation/trunk/extern/proj4/nad"
#	)
#MESSAGE("ENV : $ENV{PROJ_LIB}" )

SET( LIBRARY_PREFIX "" )
IF( WIN32 )
	SET( LIBRARY_PREFIX "" )
ENDIF(WIN32)

INCLUDE_DIRECTORIES( src/) 
ADD_EXECUTABLE(			TestOrientationMATIS_samples testing/TestOrientationMATIS_samples.cpp )
TARGET_LINK_LIBRARIES(	TestOrientationMATIS_samples ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
ADD_DEPENDENCIES(		TestOrientationMATIS_samples ${LIBRARY_PREFIX}Orientation)

ADD_EXECUTABLE(			TestOrientationMATIS_IO testing/TestOrientationMATIS_IO.cpp )
TARGET_LINK_LIBRARIES(	TestOrientationMATIS_IO ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
ADD_DEPENDENCIES(		TestOrientationMATIS_IO ${LIBRARY_PREFIX}Orientation )

ADD_EXECUTABLE(			TestOrientationMATIS_Projection testing/TestOrientationMATIS_Projection.cpp )
TARGET_LINK_LIBRARIES(	TestOrientationMATIS_Projection ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
ADD_DEPENDENCIES(		TestOrientationMATIS_Projection ${LIBRARY_PREFIX}Orientation )

ADD_EXECUTABLE(			Test_crop 						testing/Test_crop.cpp)
TARGET_LINK_LIBRARIES(	Test_crop						${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
ADD_DEPENDENCIES(		Test_crop						${LIBRARY_PREFIX}Orientation )

#ADD_EXECUTABLE(			Test_Projective 						testing/Test_Projective.cpp)
#TARGET_LINK_LIBRARIES(	Test_Projective						${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
#ADD_DEPENDENCIES(		Test_Projective						${LIBRARY_PREFIX}Orientation )


#ADD_TEST( io			TestOrientationMATIS_IO			${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.ori	${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.xml	${CMAKE_CURRENT_SOURCE_DIR}/data/MNE_892-6247_EXT.HDR	${CMAKE_CURRENT_SOURCE_DIR}/data/olivier.xml	${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_conic.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/Z_Num7_DeZoom1_SimuComplBiCub.xml)
ADD_TEST( io			TestOrientationMATIS_IO			${CMAKE_CURRENT_SOURCE_DIR}/data/olivier.xml	${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_conic.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.ori ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_WGS84.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296_crope.xml)
ADD_TEST( samples		TestOrientationMATIS_samples	${CMAKE_CURRENT_SOURCE_DIR}/data/olivier.xml	${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_conic.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/orid.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_WGS84.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/ori_test_projective.xml)
ADD_TEST( WorldToImageOldOri 	TestOrientationMATIS_Projection	${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.ori )
ADD_TEST( Transfo2DOldOri	TestOrientationMATIS_Projection	${CMAKE_CURRENT_SOURCE_DIR}/data/orid.ori )

##############################################################################
# Fin Executables de test
##############################################################################

##############################################################################
# Install
##############################################################################
INCLUDE( Orientation_install.cmake )


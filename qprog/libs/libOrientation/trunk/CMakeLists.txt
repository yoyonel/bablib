cmake_minimum_required(VERSION 2.6)

project( Orientation )

include(extern/cmake/MatisMacros.cmake)

set(ORIENTATION_ROOT_DIR ${PROJECT_SOURCE_DIR})
__default_build_configuration()
__default_install_configuration()
__general_output()

set( SRC_DIR src/ )
aux_source_directory( ${SRC_DIR} SRC_ROOT )
file( GLOB ALL_SRC_HDR ${SRC_DIR}/*.h ${SRC_DIR}/*.hpp )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/ )

##############################################################################
# Orientation
##############################################################################

option( BUILD_Orientation_SHARED "Build Orientation as a SHARED library." OFF )
if( BUILD_Orientation_SHARED )
	set( ORIENTATION_LIBRARY_TYPE SHARED )
else( BUILD_Orientation_SHARED )
	set( ORIENTATION_LIBRARY_TYPE STATIC )
endif( BUILD_Orientation_SHARED )

add_library( Orientation  ${ORIENTATION_LIBRARY_TYPE}  ${SRC_ROOT} ${ALL_SRC_HDR} )
#add_dependencies(	Orientation tinyxml proj4)

##############################################################################
# Fin Orientation
##############################################################################

##############################################################################
# BOOST
##############################################################################
# On demande a l'utilisateur si il veut utiliser ou pas BOOST
set( Boost_ADDITIONAL_VERSIONS "1.39.0" "1.39" "1.40.0" "1.40" "1.48" ) 
set( USE_BOOST ON CACHE BOOL "Use BOOST" )
if(USE_BOOST)
    set(ORIENTATIONMATIS_USE_NOBOOST 0)
    # On verifie que BOOST existe bien
    find_package( Boost COMPONENTS system filesystem date_time REQUIRED )
    if(Boost_FOUND)
    	include_directories( ${Boost_INCLUDE_DIR} )
    	link_directories( ${Boost_LIBRARY_DIRS} )
		set(Orientation_LIBRARIES ${Boost_LIBRARIES})
    else()
    	message( FATAL_ERROR "Boost not found ! Please set Boost directories !" )
        set(ORIENTATIONMATIS_USE_NOBOOST 1)
        set(USE_BOOST OFF)
    endif()
else()
    set(ORIENTATIONMATIS_USE_NOBOOST 1)
endif()
# Permet de creer automatiquement un fichier de configuration pour l'utilisation ou non de BOOST en fonction du choix fait precedemment
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config/config_boost.hpp.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/config/config_boost.hpp )
##############################################################################
# Fin BOOST
##############################################################################

##############################################################################
# PROJ4
##############################################################################

set( USE_PROJ4 ON CACHE BOOL "Use PROJ4" )
if(USE_PROJ4)
    find_package(proj4 QUIET)
    if(proj4_FOUND)
	    include_directories(${proj4_INCLUDE_DIRS})
	    message( STATUS "proj4 found!" )
        set(Orientation_LIBRARIES ${Orientation_LIBRARIES} ${proj4_LIBRARIES})
    else()
	    set(ROOT_DIR ${PROJECT_SOURCE_DIR})
	    set(proj4_INSTALL_DIR "proj4")
	    set(CUSTOM_BUILD ON)
	    set(proj4_ROOT_DIR ${ROOT_DIR}/extern/proj4)
	    include( ${proj4_ROOT_DIR}/cmake/proj4/CMakeLists.txt )
	    add_dependencies(Orientation proj4)
	    message( STATUS "proj4 not found!" )
    endif()
    set(ORIENTATIONMATIS_USE_PROJ4 1)
else()
    set(ORIENTATIONMATIS_USE_PROJ4 0)
endif()
# Permet de creer automatiquement un fichier de configuration pour l'utilisation ou non de PROJ4 en fonction du choix fait precedemment
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config/config_proj4.hpp.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/config/config_proj4.hpp )
##############################################################################
# Fin PROJ4
##############################################################################

##############################################################################
# TinyXML
##############################################################################

find_package(TinyXML QUIET)
IF(TinyXML_FOUND)
	include_directories(${TinyXML_INCLUDE_DIRS})
	add_definitions(${TinyXML_DEFINITIONS})
	message( STATUS "TinyXML found!" )
	set( Orientation_LIBRARIES ${Orientation_LIBRARIES} ${TinyXML_LIBRARIES})
ELSE()
	# Option pour selectionner une compilation sous forme de .lib ou de .dll
	option(BUILD_TINYXML_SHARED "Build tinyxml library shared." OFF)

	# Verification du type de bibliotheque a construire
	if(BUILD_TINYXML_SHARED)
	  set(TINYXML_LIBRARY_TYPE SHARED)
	else(BUILD_TINYXML_SHARED)
	  set(TINYXML_LIBRARY_TYPE STATIC)
	endif(BUILD_TINYXML_SHARED)

	# ALWAYS use STL
	add_definitions( -DTIXML_USE_STL )

	# On fixe une variable globale regroupant tous les fichiers source de tinyxml
	set( SRC_TINYXMLLIB extern/tinyxml )
	set( ALL_TINYXML_SRC_FILES ${SRC_TINYXMLLIB}/tinystr.cpp
						   ${SRC_TINYXMLLIB}/tinystr.h
						   ${SRC_TINYXMLLIB}/tinyxml.cpp
						   ${SRC_TINYXMLLIB}/tinyxml.h
						   ${SRC_TINYXMLLIB}/tinyxmlerror.cpp
						   ${SRC_TINYXMLLIB}/tinyxmlparser.cpp )
		   
	# On ajoute les repertoires d'include
	include_directories( ${SRC_TINYXMLLIB} )

	# Repertoire de sortie
	add_library( tinyxml ${TINYXML_LIBRARY_TYPE} ${ALL_TINYXML_SRC_FILES} )
	set(Orientation_LIBRARIES ${Orientation_LIBRARIES} tinyxml)

	add_dependencies(Orientation tinyxml)
	message( STATUS "TinyXML not found!" )
endif()

##############################################################################
# Fin TinyXML
##############################################################################



##############################################################################
# XERCES
##############################################################################

# Option pour utiliser xerces
SET( USE_XERCES OFF CACHE BOOL "Use Xerces" )
IF(USE_XERCES)
    set(ORIENTATIONMATIS_USE_XERCES 1)
ELSE()
    set(ORIENTATIONMATIS_USE_XERCES 0)
ENDIF()
# Permet de creer automatiquement un fichier de configuration pour l'utilisation ou non de XERCES en fonction du choix fait precedemment
CONFIGURE_FILE( ${CMAKE_CURRENT_SOURCE_DIR}/config/config_xerces.hpp.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/config/config_xerces.hpp )

##############################################################################
# Fin XERCES
##############################################################################



##############################################################################
# CGAL
##############################################################################

# Option pour utiliser CGAL
set( USE_CGAL OFF CACHE BOOL "Use CGAL" )
if(USE_CGAL)
    find_package( CGAL COMPONENTS Core )
    if( CGAL_FOUND )
	    include( ${CGAL_USE_FILE} )
        set(ORIENTATIONMATIS_USE_CGAL 1)
        set( Orientation_LIBRARIES ${Orientation_LIBRARIES} ${CGAL_LIBRARIES})
    else()
	    message( STATUS "CGAL not found!" )
        set(ORIENTATIONMATIS_USE_CGAL 0)
    endif()
else()
    set(ORIENTATIONMATIS_USE_CGAL 0)
endif()
# Permet de creer automatiquement un fichier de configuration pour l'utilisation ou non de CGAL en fonction du choix fait precedemment
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/config/config_cgal.hpp.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/config/config_cgal.hpp )

##############################################################################
# Fin CGAL
##############################################################################

target_link_libraries(Orientation ${Orientation_LIBRARIES})
set_target_properties(Orientation PROPERTIES DEBUG_POSTFIX "-d")

##############################################################################
# Samples
##############################################################################
option(BUILD_SAMPLES "Build ${PROJECT_NAME} samples." OFF)
if(BUILD_SAMPLES)
    add_executable(convert samples/convert.cpp )
    target_link_libraries(convert ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    add_dependencies(convert ${LIBRARY_PREFIX}Orientation)
    message(STATUS "Added target convert (exe)")
endif(BUILD_SAMPLES)
##############################################################################
# Fin samples
##############################################################################

##############################################################################
# Executables de test
##############################################################################
option( BUILD_TESTS "Build tests" ON )
if( BUILD_TESTS )

    link_directories( ${Boost_LIBRARY_DIRS} )
    enable_testing()
    
    include_directories( src/) 

    add_executable(TestOrientationMATIS_NonValidFile testing/TestOrientationMATIS_NonValidFile.cpp )
    target_link_libraries(TestOrientationMATIS_NonValidFile ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    add_dependencies(TestOrientationMATIS_NonValidFile ${LIBRARY_PREFIX}Orientation)


    add_executable(			TestOrientationMATIS_samples testing/TestOrientationMATIS_samples.cpp )
    target_link_libraries(	TestOrientationMATIS_samples ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    add_dependencies(		TestOrientationMATIS_samples ${LIBRARY_PREFIX}Orientation)

    add_executable(			TestOrientationMATIS_IO testing/TestOrientationMATIS_IO.cpp )
    target_link_libraries(	TestOrientationMATIS_IO ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    add_dependencies(		TestOrientationMATIS_IO ${LIBRARY_PREFIX}Orientation )

    add_executable(			TestOrientationMATIS_Projection testing/TestOrientationMATIS_Projection.cpp )
    target_link_libraries(	TestOrientationMATIS_Projection ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    add_dependencies(		TestOrientationMATIS_Projection ${LIBRARY_PREFIX}Orientation )

    add_executable(			Test_crop 						testing/Test_crop.cpp)
    target_link_libraries(	Test_crop						${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    add_dependencies(		Test_crop						${LIBRARY_PREFIX}Orientation )

    #add_executable(			Test_Projective 						testing/Test_Projective.cpp)
    #target_link_libraries(	Test_Projective						${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    #add_dependencies(		Test_Projective						${LIBRARY_PREFIX}Orientation )

    add_executable(Test_PairOrientation    testing/TestPairOrientation.cpp)
    target_link_libraries(Test_PairOrientation    ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    add_dependencies(Test_PairOrientation    ${LIBRARY_PREFIX}Orientation )

    add_executable(Test_OldOriWriting    testing/TestOldOriWriting.cpp)
    target_link_libraries(Test_OldOriWriting    ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    add_dependencies(Test_OldOriWriting    ${LIBRARY_PREFIX}Orientation )

    #add_executable(TestOrientationMATIS_ProjectionCarto testing/TestOrientationMATIS_ProjectionCarto.cpp )
    #target_link_libraries(TestOrientationMATIS_ProjectionCarto ${Orientation_LIBRARIES} ${LIBRARY_PREFIX}Orientation)
    #add_dependencies(TestOrientationMATIS_ProjectionCarto ${LIBRARY_PREFIX}Orientation)


    #add_test( io			TestOrientationMATIS_IO			${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.ori	${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.xml	${CMAKE_CURRENT_SOURCE_DIR}/data/MNE_892-6247_EXT.HDR	${CMAKE_CURRENT_SOURCE_DIR}/data/olivier.xml	${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_conic.xml ${CMAKE_CURRENT_SOURCE_DIR}/data/Z_Num7_DeZoom1_SimuComplBiCub.xml)




    add_test( io_NonValidFiles ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestOrientationMATIS_NonValidFile
    # ${CMAKE_CURRENT_SOURCE_DIR}/data/Pano.xml  ## implémentation de spheric_xml_reader  FC
     ${CMAKE_CURRENT_SOURCE_DIR}/data/RoadSigns.xml)


    add_test( io
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestOrientationMATIS_IO			
    ${CMAKE_CURRENT_SOURCE_DIR}/data/olivier.xml 
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_conic.xml 
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.ori
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.xml 
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_WGS84.xml 
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296_crope.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Mairie-090708_0645-22-00001_0000704_1.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Mairie-090708_0645-34-00001_0000706.ori.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/data/ORTHO.HDR
    )
    
    add_test( samples
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestOrientationMATIS_samples	
   	${CMAKE_CURRENT_SOURCE_DIR}/data/Sample_WGS84.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/data/olivier_Projective.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.xml 
    ${CMAKE_CURRENT_SOURCE_DIR}/data/orid.xml  
    ${CMAKE_CURRENT_SOURCE_DIR}/data/ori_test_projective.xml
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Pano_SampleFC.xml  ## implémentation de spheric_xml_reader  FC
    )
    
    add_test( WorldToImageOldOri
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestOrientationMATIS_Projection	
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.ori 
   )
    
    add_test( Transfo2DOldOri
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/TestOrientationMATIS_Projection	
    ${CMAKE_CURRENT_SOURCE_DIR}/data/orid.ori )
    
    add_test(PairOrientation
	${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Test_PairOrientation    
    ${CMAKE_CURRENT_SOURCE_DIR}/data/st_antoine.txt    
    ${CMAKE_CURRENT_SOURCE_DIR}/data/orig_st_antoine.ori 
    ${CMAKE_CURRENT_SOURCE_DIR}/data/orid_st_antoine.ori)


    add_test(Test_OldOriWriting
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Test_OldOriWriting
    ${CMAKE_CURRENT_SOURCE_DIR}/data/Amiens296.ori
     ${CMAKE_CURRENT_SOURCE_DIR}/data/orid.ori
    )

    
endif()
##############################################################################
# Fin Executables de test
##############################################################################
##############################################################################
# doxygen documentation
##############################################################################
include(Orientation-doc.cmake)
##############################################################################
##############################################################################
# Install
##############################################################################
include(Orientation_install.cmake)


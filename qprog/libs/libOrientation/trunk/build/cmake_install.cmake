# Install script for directory: /home/atty/Prog/libOrientation/trunk

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/Orientation" TYPE FILE FILES
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojectionrto.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/geodesieproj4.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojectionvisitor.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/projectivetransfo2D.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/systematismecylindrique.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/geometrictransformation.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojectionconique.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/systematismeaffine.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/transfo2delementaire.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/euclidien.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/ConicStereoPair.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/noboost.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/distortiongrillematis.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/transfo2d.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/homographie2d.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/geodesie.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/auxiliarydata.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojectioncarto.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojectionaxono.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/outil_xml.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojectiongrille.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/distortionpolynomeign.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojectionspherique.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojectionphysique.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/modeleprojection.hpp"
    "/home/atty/Prog/libOrientation/trunk/src/solar.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/include/Orientation/config" TYPE FILE FILES
    "/home/atty/Prog/libOrientation/trunk/config/config_boost.hpp"
    "/home/atty/Prog/libOrientation/trunk/config/config_cgal.hpp"
    "/home/atty/Prog/libOrientation/trunk/config/config_xerces.hpp"
    "/home/atty/Prog/libOrientation/trunk/config/config_proj4.hpp"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/lib" TYPE STATIC_LIBRARY FILES "/home/atty/Prog/libOrientation/trunk/build/lib/libOrientation.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/usr/local/cmake/Orientation-targets.cmake")
    FILE(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}/usr/local/cmake/Orientation-targets.cmake"
         "/home/atty/Prog/libOrientation/trunk/build/CMakeFiles/Export/_usr/local/cmake/Orientation-targets.cmake")
    IF(EXPORT_FILE_CHANGED)
      FILE(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}/usr/local/cmake/Orientation-targets-*.cmake")
      IF(OLD_CONFIG_FILES)
        MESSAGE(STATUS "Old export file \"$ENV{DESTDIR}/usr/local/cmake/Orientation-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        FILE(REMOVE ${OLD_CONFIG_FILES})
      ENDIF(OLD_CONFIG_FILES)
    ENDIF(EXPORT_FILE_CHANGED)
  ENDIF()
  FILE(INSTALL DESTINATION "/usr/local/cmake" TYPE FILE FILES "/home/atty/Prog/libOrientation/trunk/build/CMakeFiles/Export/_usr/local/cmake/Orientation-targets.cmake")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "/usr/local/cmake" TYPE FILE FILES "/home/atty/Prog/libOrientation/trunk/build/CMakeFiles/Export/_usr/local/cmake/Orientation-targets-release.cmake")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "/usr/local/cmake" TYPE FILE FILES "/home/atty/Prog/libOrientation/trunk/build/OrientationConfig.cmake")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/home/atty/Prog/libOrientation/trunk/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/home/atty/Prog/libOrientation/trunk/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)

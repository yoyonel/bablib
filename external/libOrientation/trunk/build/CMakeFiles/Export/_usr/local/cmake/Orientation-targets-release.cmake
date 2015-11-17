#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
SET(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Orientation" for configuration "Release"
SET_PROPERTY(TARGET Orientation APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
SET_TARGET_PROPERTIES(Orientation PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "/usr/lib/libboost_system-mt.so;/usr/lib/libboost_filesystem-mt.so;/usr/lib/libboost_date_time-mt.so;proj4;TinyXML_STL"
  IMPORTED_LOCATION_RELEASE "/usr/local/lib/libOrientation.a"
  )

# Commands beyond this point should not need to know the version.
SET(CMAKE_IMPORT_FILE_VERSION)

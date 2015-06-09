IF( UNIX )
    SET(CMAKE_INSTALL_SO_NO_EXE "0")
  # Si la lib est dynamique
  IF( BUILD_Orientation_SHARED )
    INSTALL( TARGETS Orientation DESTINATION lib )
  ELSE( )
    INSTALL( TARGETS Orientation ARCHIVE DESTINATION lib )
  ENDIF( )
  
####
#### On recherche tous les fichiers ".h", ".hpp", ".hxx" et "*.inl"
####
FILE( GLOB ALL_HEADER_FILES src/*.h src/*.hxx src/*.hpp )
#MESSAGE(FATAL_ERROR ${ALL_HEADER_FILES} )
INSTALL( FILES ${ALL_HEADER_FILES} DESTINATION include/Orientation )

ENDIF( )

IF( WIN32 )
  # TODO !
ENDIF( )
 

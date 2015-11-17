cmake_minimum_required (VERSION 2.6)
project (Bablib_Externals)

#############
# GLEW      #
#############
ADD_SUBDIRECTORY( glew )
INCLUDE_DIRECTORIES( glew/src/. )


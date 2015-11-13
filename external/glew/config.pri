# build from glew 1.5.1 sources (glew-1.5.1-src.tgz -- http://glew.sourceforge.net/)

LOCAL_PATH = $$PWD

DEFINES *= GLEW_STATIC      # pour construire lib statique
#DEFINES *= GLEW_BUILD      # pour construire lib dynamique

LIB_HEADERS     = $$LOCAL_PATH/src/GL/*.h   $$LOCAL_PATH/src/*.h
LIB_SOURCES     = $$LOCAL_PATH/src/*.c $$LOCAL_PATH/src/*.cpp
LIB_INCLUDEPATH = $$LOCAL_PATH/src


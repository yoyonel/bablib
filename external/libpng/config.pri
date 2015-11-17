# lib construite a partir des sources de libpng 1.2.40 (http://www.libpng.org/pub/png/libpng.html)
# = libpng-1.2.40-no-config.tar.gz
# (copie telle quelle des sources sans modif)

LOCAL_PATH = $$PWD

LIB_HEADERS     = $$LOCAL_PATH/sources/*.h
LIB_SOURCES     = $$LOCAL_PATH/sources/*.c
LIB_INCLUDEPATH = $$LOCAL_PATH/sources

CONFIG *= zlib  # needed by libpng


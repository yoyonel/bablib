# lib construite a partir des sources de zlib 1.2.3 (http://www.zlib.net/)
# = zlib-1.2.3.tar.gz
# (copie telle quelle des sources sans modif)

LOCAL_PATH = $$PWD

LIB_HEADERS     = $$LOCAL_PATH/sources/*.h
LIB_SOURCES     = $$LOCAL_PATH/sources/*.c
LIB_INCLUDEPATH = $$LOCAL_PATH/sources


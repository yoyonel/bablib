TARGET   = View
TEMPLATE = app
CONFIG  *= warn_on debug

CONFIG *= bablib

HEADERS = sources/*.h
SOURCES = sources/*.cpp

#ajout pour compil avec mingw :
win32 {
    SOURCE_LIBS *= bablib glew
    DEFINES *= GLEW_STATIC      # pour construire lib statique
    CONFIG *= warn_on thread console
    CONFIG -= debug_and_release
    CONFIG -= debug
    CONFIG *= release
    }

PROJECT_PATH = $$PWD

include($$[QPROG]/config_project.pri)

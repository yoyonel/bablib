TARGET   = View
TEMPLATE = app
CONFIG  *= warn_on debug

CONFIG *= gsl
CONFIG *= scene3d
CONFIG *= bablib

HEADERS = sources\*.h
SOURCES = sources\*.cpp

PROJECT_PATH = $$PWD

include($$[QPROG]/config_project.pri)
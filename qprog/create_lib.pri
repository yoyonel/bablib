# ce script permet de créer une lib statique en utilisant le config.pri contenu dans le répertoire de la lib
# la variable CURRENT_LIB définit le nom de la lib à créer

isEmpty(CURRENT_LIB):\
    error("la variable CURRENT_LIB doit être définie, et contenir le nom de la lib à compiler")

# le repertoire de build pour les libs statiques :
BUILD_DIR = $$join(CURRENT_LIB,,libstatic_,)

include(config_project.pri)
    
isEmpty(COMPILED_LIBS_PATH):\
    error("la variable COMPILED_LIBS_PATH doit être définie (par le config_project.pri qui doit être inclus avant)")
isEmpty(LIBS_PATH):\
    error("la variable LIBS_PATH doit être définie (par le config_project.pri qui doit être inclus avant)")

TEMPLATE = lib
CONFIG  *= staticlib
TARGET   = $$CURRENT_LIB
DESTDIR  = $$COMPILED_LIBS_PATH
VERSION  = 1.0

LIB_SOURCES =
LIB_HEADERS =
LIB_INCLUDEPATH =
include($$LIBS_PATH/$$CURRENT_LIB/config.pri)
SOURCES *= $$LIB_SOURCES
HEADERS *= $$LIB_HEADERS
INCLUDEPATH *= $$LIB_INCLUDEPATH

ONLY_LIBS_HEADERS = true    # because only headers of external libs are needed to link this static lib

# par defaut la lib est construite en release :
CONFIG -= debug
CONFIG *= release


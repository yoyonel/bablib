# ce script définit les variables LIB_HEADERS, LIB_SOURCES et LIB_INCLUDEPATH
# ainsi que les dépendances aux autres libs

LOCAL_PATH = $$PWD

LIB_HEADERS += $$LOCAL_PATH/src/bablib/*.h
LIB_SOURCES += $$LOCAL_PATH/src/bablib/*.cpp

## following sources should be recompiled with each project, as they depend on some project-wise defines :
#LIB_HEADERS_DYN += $$LOCAL_PATH/src/bablib/Constants.h
#LIB_SOURCES_DYN += $$LOCAL_PATH/src/bablib/Constants.cpp

# declaration en dehors du "contains" pour contourner problème de parsing des {} par qmake :
BABLIB_NAMESPACE_OPEN  = '"BABLIB_NAMESPACE_BEGIN=namespace bablib{"'
BABLIB_NAMESPACE_CLOSE = '"BABLIB_NAMESPACE_END=}"'

#BABLIB_CONFIG *= namespace
# attention : si la lib est précompilée en lib statique, ne pas modifier BABLIB_CONFIG dans le .pro du projet,
# sinon gestion du namespace potentiellement incohérente
contains(BABLIB_CONFIG, namespace) {
    DEFINES *= BABLIB_NAMESPACE
    DEFINES *= $$BABLIB_NAMESPACE_OPEN
    DEFINES *= $$BABLIB_NAMESPACE_CLOSE
    DEFINES *= '"BABLIB_NAMESPACE_USE=using namespace bablib;"'
    DEFINES *= '"BABLIB_NAMESPACE_NAME(name)=bablib::name"'
    LIB_INCLUDEPATH += $$LOCAL_PATH/src
    }
else {
    DEFINES *= BABLIB_NAMESPACE_BEGIN=
    DEFINES *= BABLIB_NAMESPACE_END=
    DEFINES *= BABLIB_NAMESPACE_USE=
    DEFINES *= '"BABLIB_NAMESPACE_NAME(name)=name"'
    LIB_INCLUDEPATH += $$LOCAL_PATH/src/bablib
    }

## definition of the shaders lib's path, required by the ProgGLSL class :
#DEFINES *= $$sprintf("BABLIB_SHADERS_LIB_DIR=%1", $$LOCAL_PATH/shaders)

!win32 {
    DEFINES *= BABLIB_GSL_SUPPORT
    CONFIG *= gsl
    }

# définition des dependances :
CONFIG *= constants
CONFIG *= glew
CONFIG *= qglviewer
CONFIG *= qt
CONFIG *= libpng
CONFIG *= boost_pp
QT *= xml opengl

QMAKE_CXXFLAGS += -fpermissive
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra

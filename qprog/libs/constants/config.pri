# ce script définit les variables LIB_HEADERS, LIB_SOURCES et LIB_INCLUDEPATH
# ainsi que les dépendances aux autres libs

LOCAL_PATH = $$PWD

LIB_HEADERS += $$LOCAL_PATH/*.h
LIB_SOURCES += $$LOCAL_PATH/*.cpp
LIB_INCLUDEPATH = $$LOCAL_PATH

# declaration en dehors du "contains" pour contourner problème de parsing des {} par qmake :
#BABLIB_NAMESPACE_OPEN  = '"BABLIB_NAMESPACE_BEGIN=namespace bablib{"'
#BABLIB_NAMESPACE_CLOSE = '"BABLIB_NAMESPACE_END=}"'

#    DEFINES *= '"BABLIB_NAMESPACE_USE=using namespace bablib;"'
#    DEFINES *= '"BABLIB_NAMESPACE_NAME(name)=bablib::name"'


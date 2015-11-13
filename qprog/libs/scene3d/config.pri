# ce script dÃ©finit les variables LIB_HEADERS, LIB_SOURCES et LIB_INCLUDEPATH
# ainsi que les dÃ©pendances aux autres libs

LOCAL_PATH = $$PWD

DEFINES *= PLY_SUPPORT

LIB_HEADERS =                               \
    $$LOCAL_PATH/src/scene3d/*.h        \
    $$LOCAL_PATH/src/scene3d/3ds/*.h    \
    $$LOCAL_PATH/src/scene3d/off/*.h    \
    $$LOCAL_PATH/src/scene3d/ply/*.h    \
    $$LOCAL_PATH/src/scene3d/obj/*.h

#    $$LOCAL_PATH/sources/wrl/*.h    \

LIB_SOURCES =                           \
    $$LOCAL_PATH/src/scene3d/*.cpp      \
    $$LOCAL_PATH/src/scene3d/3ds/*.cpp  \
    $$LOCAL_PATH/src/scene3d/off/*.cpp  \
    $$LOCAL_PATH/src/scene3d/ply/*.c    \
    $$LOCAL_PATH/src/scene3d/ply/*.cpp  \
    $$LOCAL_PATH/src/scene3d/obj/*.cpp

#    $$LOCAL_PATH/sources/wrl/*.cpp  \

LIB_INCLUDEPATH = $$LOCAL_PATH/src

# dÃ©finition des dependances :
CONFIG *= bablib
#CONFIG *= glide_assets
#CONFIG *= glide_scene
CONFIG *= glew
CONFIG *= lib3ds


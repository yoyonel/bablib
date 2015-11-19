TARGET   = View
TEMPLATE = app

CONFIG *= bablib
CONFIG *= scene3d
CONFIG *= gsl

HEADERS = sources/*.h \
    sources/TriSoup2.h \
    sources/SolidVoxelisation.h

SOURCES = sources/*.cpp

OTHER_FILES +=                                              \
                shaders/vbo/vbo.frag                        \
                shaders/vbo/vbo.vert                        \
		shaders/vbo_wire/vbo_wire.frag              \
		shaders/vbo_wire/vbo_wire.vert              \
                shaders/vbo_sv/vbo_sv.frag                  \
                shaders/vbo_sv/vbo_sv.vert                  \
                shaders/draw_texture/draw_texture.frag      \
                shaders/draw_texture/draw_texture.vert      \
                shaders/outils/defines.glsl                 \
                shaders/outils/functions.glsl               \
                default.par                                 \
                notes.txt \
    shaders/outils/defines.glsl \
    shaders/outils/area_light.glsl \
    screenshots/*.png

PROJECT_PATH = $$PWD

# http://stackoverflow.com/questions/5715543/how-to-change-qmake-release-flags-for-gcc-change-o2-to-os
QMAKE_CXXFLAGS_RELEASE -= -Wint-to-pointer-cast

include($$[QPROG]/config_project.pri)
QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra

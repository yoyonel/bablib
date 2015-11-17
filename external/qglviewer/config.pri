#LIB_PATH = $$PWD/libQGLViewer-2.3.16
#LIB_PATH = $$PWD/libQGLViewer-2.3.5
#LIB_PATH = $$PWD/libQGLViewer-2.3.7
#LIB_PATH = $$PWD/libQGLViewer-2.3.9
LIB_PATH = $$PWD/libqglviewer

INCLUDEPATH *= $$LIB_PATH
DEPENDPATH  *= $$LIB_PATH

unix {
    LIBS *= -Wl,"-rpath=$$LIB_PATH/QGLViewer" \
            -L$$LIB_PATH/QGLViewer \
            -lQGLViewer
    }
else {
    LIB_DIR = $$LIB_PATH/QGLViewer/release
    isEmpty( QGLVIEWER_STATIC ) {
        #LIBS *= -L$${LIB_DIR} -lQGLViewer
        LIBS *= -L$${LIB_DIR} -lQGLViewer2
        }
    else {
        LIB_NAME = libQGLViewer*.a
        LIBS *= $${LIB_DIR}/$${LIB_NAME}
        }
    }

CONFIG *= qt
QT *= opengl xml


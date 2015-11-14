#ifndef __OPEN_GL__
#define __OPEN_GL__

#include "GL/glew.h"


class OpenGL {
    public:
        static void init();     // doit être appelé dès que le contexte OpenGL est crée (ex: init du QGLViewer)
    };

#endif

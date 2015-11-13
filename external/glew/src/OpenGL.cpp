#include "OpenGL.h"
#include <stdio.h>

void OpenGL::init() {
    //glutInit(&argc, argv);
    //glutCreateWindow("GLEW Test");
    
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        // problem : glewInit failed, something is seriously wrong.
        fprintf(stderr, "glew init error : %s\n", glewGetErrorString(error));
        }
    //fprintf(stdout, "glew status : using GLEW %s\n", glewGetString(GLEW_VERSION));
    }


#include "viewer_sv.h"

#include <Message.h>


void Viewer::destroyAll()
{
    destroyShaders();
    destroyTextures();
    destroyFrameBuffers();
    //
    destroyTriSoup();
    destroyVBO();
    //
    destroyTimerQueryNV();
    //
    if( solid_voxelisation ) {
        delete solid_voxelisation;
    }

    b_is_init = false;
}

void Viewer::destroyShaders()
{
    prog_vbo.destroy();
    prog_vbo_wire.destroy();
    prog_vbo_sv.destroy();
    prog_draw_texture.destroy();
}

void Viewer::destroyTextures()
{
}

void Viewer::destroyFrameBuffers()
{
}

void Viewer::destroyTimerQueryNV()
{
}

void Viewer::destroyTriSoup()
{
    delete ts;
}

void Viewer::destroyVBO()
{
    delete vbo;
    delete indexBuffer;
}


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
    //printf("# destroyTexturesForShadowMap\n");
    tex_sv.destroy();
}

void Viewer::destroyFrameBuffers()
{
    //printf("# destroyFrameBufferForShadowMap\n");
    // - FrameBuffer pour rendre la Depth Shadow Map (render_to_depth_texture)
    fb_sv.destroy();
}

void Viewer::destroyTimerQueryNV()
{
    //printf("# destroyTimerQueryNV\n");
}

void Viewer::destroyTriSoup()
{
    delete ts;
}

void Viewer::destroyVBO()
{
    delete vbo;
    delete indexBuffer;
    //
    //	delete indexBuffer_sv;
    //	delete vbo_sv;	 //[MOG: segfault quand on recharge]
}


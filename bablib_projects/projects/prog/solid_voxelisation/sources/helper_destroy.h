#ifndef __HELPER_DESTROY_H__
#define __HELPER_DESTROY_H__

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
}

void Viewer::destroyTextures()
{
    //printf("# destroyTexturesForShadowMap\n");
}

void Viewer::destroyFrameBuffers()
{
	//printf("# destroyFrameBufferForShadowMap\n");
		// - FrameBuffer pour rendre la Depth Shadow Map (render_to_depth_texture)
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

#endif

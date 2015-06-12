#ifndef __HELPER_DESTROY_H__
#define __HELPER_DESTROY_H__

void Viewer::destroyAll()
{
	destroyShaders();
	destroyTextures();
	destroyFrameBuffers();
}

void Viewer::destroyShaders()
{
	prog_mipmap.destroy();
	printf("# destroyShaders\n");
}

void Viewer::destroyTextures()
{
	if (imgTex.isLoaded()) imgTex.destroy();
	printf("# destroyTexturesForShadowMap\n");
	
}

void Viewer::destroyFrameBuffers()
{
	printf("# destroyFrameBufferForShadowMap\n");
	
}

#endif

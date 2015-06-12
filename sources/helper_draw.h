#ifndef __HELPER_VIEWER_DRAW__
#define __HELPER_VIEWER_DRAW__

void Viewer::drawQuad(float sz, float texc)
{	
	const float data_T2F_V3F[][5] = {	
		{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, 
		{ texc, 0.0f, sz, 0.0f, 0.0f }, 
		{ texc, texc, sz, sz, 0.0f }, 
		{ 0.0f, texc, 0.0f, sz, 0.0f } 
	};	

	glInterleavedArrays(GL_T2F_V3F, 0, data_T2F_V3F);
	glDrawArrays(GL_QUADS, 0, 4);
}

void Viewer::drawTexture( int x, int y, int w, int h, ProgGLSL& _prog, Texture& _tex ) {	
	glPushClientAttrib(GL_ALL_ATTRIB_BITS);	
	glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);					

	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(x, y, w, h);

	// - active le shader et la texture associee
	_prog.activate();
	_prog.activateTextures();
	
	static float t = 0.0;
	t += 1.f/60.f;
	float coefLod = sin(t)*sin(t) * 5;
	_prog.setUniform( "coefLod", coefLod, false);

	float tex_width 	= _tex.getWidth();
	float tex_height 	= _tex.getHeight();
	_prog.setUniform( "tex_width",  tex_width, false );
	_prog.setUniform( "tex_height", tex_height, false );
	
	drawQuad( 1.f, 1.f );

	// - desactive le shader et la texture associee
	_prog.deactivate();

	// - restaure les matrices OpenGL
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();
	
	// - restaure les attributs
	glPopClientAttrib();		
}

#endif